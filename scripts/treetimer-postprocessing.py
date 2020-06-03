#!/usr/bin/python

# Post-processing of treetimer database data

# Select the desired functionality from the main method below by uncommenting the method call
# (todo - add a command line selector!)

# Functionality Needed

# (1) Display Full CallTree (Functions only) alongside exclusive time spent in each function
#	 (Some functions may not be timed individually due to overhead, and thus will show
#	  as time under another function)

# (2) Display Hotspots by Function Exclusive Time (Ordered Version of (1))

# (6) Validate that all function time is captured (summation == walltime)
# (7) Validate that all non function time is captured (i.e. functions are suitable broken down into compute, comms blocks)

# (3) Display Hotspots by Compute Time

# (4) Display Compute/MPI Breakdown for a run

# (5) Display Time by Algorithmic Purpose (domain-specific - e.g. exchange components, combustion, spray etc)


# (8) Generate Wgs for Compute Blocks (time/suitable parameters)

import sys, os, re
from pprint import pprint

import sqlite3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--tt-results-dirpath', required=True, help="Dirpath to 'tt_results' folder")
args = parser.parse_args()

def main():
	tt_folder_dirpath = args.tt_results_dirpath
	if not os.path.isdir(tt_folder_dirpath):
		raise Exception("Folder does not exist: {0}".format(tt_folder_dirpath))
	df_all = None
	rank_ids = set()
	for run_root, run_dirnames, run_filenames in os.walk(tt_folder_dirpath):
		for f in run_filenames:
			m = re.match("^results\.([0-9]+)\.db$", f)
			if m:
				rank = m.groups()[0]
				# rank_ids.add(rank)
				# df = load_db(os.path.join(tt_folder_dirpath, f))
				# df["rank"] = rank
				# if df_all is None:
				# 	df_all = df
				# else:
				# 	df_all = df_all.append(df)


				db = sqlite3.connect(os.path.join(tt_folder_dirpath, f))
				## Function 1
				## Display Hotspots by Grouped Node Type Exclusive Time
				## This will group all of the nodes of specific types - e.g. MPI call, compute etc.
				## UPDATE: Function 1 does not behave as described.
				# printSummaryExclusiveTimeByType(1, 1, db)

				## Function 2
				## Display Full CallTree (All CallPath Nodes) alongside exclusive time spent in each function
				## Note: There is no guarantee that the ordering is in order of first called (this is only
				## tracked in trace mode), but all blocks will be correctly nested under blocks from which they were
				## called. An indention indicates a nested block inside another block.
				# printAggregateDetailsCallPathTree(1, 1, db)
				t = buildCallPathTree(1, 1, db)
				# pprint(t)
				# quit()

				fig_dims = (4,4)
				fig = plt.figure(figsize=fig_dims)
				# sunburst(t)
				sunburst_root(t)
				plt.savefig("chart.png")
				plt.close(fig)

				quit()

	df_all["num_ranks"] = len(rank_ids)

	# df_all.sort_values("ExclusiveAggregateTime(s)", inplace=True, ascending=False)
	df_all.sort_values(["Name", "rank"], inplace=True)

	# df_filename = "hotspotsAggregateExclusiveTime.csv"
	df_filename = os.path.join(tt_folder_dirpath, "hotspotsAggregateExclusiveTime.csv")
	df_all.to_csv(df_filename, index=False)
	# print("Data written to '{0}'".format(df_filename))

def load_db(db_filepath):

	# Open Database
	db = sqlite3.connect(db_filepath)

	df = loadHotspotsAggregateTime(1, 1, db)

	# Close Database
	db.close()

	return df

# Note - RunID is the SQL Key for the RunID, and processID is the SQL Key ProcessID in the db, not the process rank! (They may not be the same)
def loadHotspotsAggregateTime(runID, processID, db):

	# Get Root Details
	rootID = getRootCallPathID(db)
	rootRecord = getAggregateCallPathNodeDetails(rootID, runID, processID, db)

	records = getAllProfileNodesAggregateTimeExclusive(runID, processID, db)

	col_names = ["Name", "TypeName", "CallCount", "InclusiveAggregateTime(s)", "ExclusiveAggregateTime(s)", "ExclusiveAggregateTime(% of Run)"]
	df = None

	sum = 0.0
	nonfunctionsum = 0.0
	for r in records:
		datum = {"Name": r['Name'], 
				"TypeName": r['TypeName'], 
				"CallCount": r['CallCount'], 
				"InclusiveAggregateTime(s)": r['AggTotalTimeI'], 
				"ExclusiveAggregateTime(s)": r['AggTotalTimeE'], 
				"ExclusiveAggregateTime(% of Run)": (r['AggTotalTimeE']/rootRecord['AggTotalTimeI']) }

		if df is None:
			df_init = {k:[v] for k,v in datum.items()}
			df = pd.DataFrame(data=df_init, columns=col_names)
		else:
			datum = [r['Name'], 
					 r['TypeName'], 
					 r['CallCount'], 
					 r['AggTotalTimeI'], 
					 r['AggTotalTimeE'], 
					 r['AggTotalTimeE']/rootRecord['AggTotalTimeI'] ]
			datum = pd.Series(datum, index=col_names)
			df = df.append(datum, ignore_index=True)

		sum = sum + r['AggTotalTimeE']

		if(r['TypeName'] != 'Method'):
			nonfunctionsum = nonfunctionsum + r['AggTotalTimeE']

	return df

# Note - RunID is the SQL Key for the RunID, and processID is the SQL Key ProcessID in the db, not the process rank! (They may not be the same)
def getAllProfileNodesAggregateTimeExclusive(runID, processID, db):

	# Since aggregate properties operate on call path nodes (since
	# times for different paths to the same profile node are stored
	# separately), we need to merge these values.

	# First, identify the call path nodes associated with a run + process
	db.row_factory = sqlite3.Row
	cur = db.cursor()

	cmd = "SELECT DISTINCT CallPathID FROM AggregateTime " + \
		  "WHERE RunID = " + str(runID) + " AND " + \
		  "ProcessID = " + str(processID)

	cur.execute(cmd)
	result = cur.fetchall()

	# Get Aggregate Data
	records = []

	for callPaths in result:
		data = getAggregateCallPathNodeDetails(callPaths['CallPathID'], runID, processID, db)
		data['CallPathID'] = callPaths['CallPathID']
		records.append(data)

	# Sort on profile node name - would ideally like to do this in database,
	# but we need to compute the exclusive time
	sortedRecords = sorted(records, key=lambda k: k['Name'])

	# Group the values together
	groupRecords = []
	group = ''
	count = -1
	for i in sortedRecords:
		if(group == "" or group != i['Name']):
			# Add record to list
			groupRecords.append(i)
			group = i['Name']
			count = count + 1
		else:
			# A record with the same name exists, append the values onto this one
			groupRecords[count]['AggMinTime'] += i['AggMinTime']
			groupRecords[count]['AggAvgTime'] += i['AggAvgTime']
			groupRecords[count]['AggMaxTime'] += i['AggMaxTime']
			groupRecords[count]['CallCount'] += i['CallCount']
			groupRecords[count]['AggTotalTimeI'] += i['AggTotalTimeI']
			groupRecords[count]['AggTotalTimeE'] += i['AggTotalTimeE']

	# We now have a list of nodes with the grouped values
	# Return the list
	return groupRecords

# Get the CallPathID of the root walltime node
# Note: This is the only special case, other Call Path Nodes may appear
# multiple times for the same profile node, since they could have different parent
# nodes for a different run
# Since root has no parent nodes, this should only ever appear once and is likely
# ID 1, but don't wish to assume
# This is also the only profile/callpath node that is guaranteed to exist
def getRootCallPathID(db):
	db.row_factory = sqlite3.Row
	cur = db.cursor()

	cmd = "SELECT CallPathID FROM CallPathData NATURAL JOIN ProfileNodeData " + \
		  "WHERE NodeName LIKE 'ProgramRoot'"

	cur.execute(cmd)
	result = cur.fetchone()

	return result['CallPathID']


# Traverse the call path tree for a run, and print the aggregate details for each
# call path node
# Indented to show depth of call in stack
# Note - RunID is the SQL Key for the RunID, and processID is the SQL Key ProcessID in the db, not the process rank! (They may not be the same)
def printAggregateDetailsCallPathTree(runID, processID, db):

	# Start at root
	rootID = getRootCallPathID(db)

	print('Depth & ID ,Name ,TypeName ,AggMinTime ,AggAvgTime ,AggMaxTime ,CallCount ,AggTotalTimeI ,AggTotalTimeE')
	printAggregateDetailsCallPathNodeTraversal(runID, rootID, processID, db, 0)

def printAggregateDetailsCallPathNodeTraversal(runID, callPathID, processID, db, indentLevel):
	# Recursive method to do depth first traversal when printing nodes

	for i in xrange(0, indentLevel):
		sys.stdout.write('  ')

#	getAggregateCallPathNodeDetails(callPathID, runID, processID, db)
	sys.stdout.write(str(callPathID) + ",")
	printAggregateCallPathNodeDetails(callPathID, runID, processID, db)
	childNodes = getCallPathChildNodeIDs(callPathID, db)

	# Loop over child nodes
	for childID in childNodes:
		# Keep following a path till we reach a node with no children
		printAggregateDetailsCallPathNodeTraversal(runID, childID, processID, db, indentLevel + 1)


def buildCallPathTree(runID, processID, db):
	rootID = getRootCallPathID(db)
	tree = None
	x = buildCallPathNodeTraversal(runID, processID, db, tree, rootID, 0)
	return x

def buildCallPathNodeTraversal(runID, processID, db, treeNode, nodeID, indentLevel):
	# Recursive depth-first traversal through call stack
	record = getAggregateCallPathNodeDetails(nodeID, runID, processID, db)

	# leaf = [record["Name"], record["TypeName"], record["AggTotalTimeI"], [] ]
	# leaf = [ ( record["Name"], record["TypeName"], record["AggTotalTimeI"], [] ) ]
	leaf = ( record["Name"], record["TypeName"], record["AggTotalTimeI"], [] )
	am_root = False
	if treeNode is None:
		am_root = True
		# treeNode = leaf
		treeNode = [ leaf ]
	else:
		# pprint(treeNode)
		treeNode[-1].append(leaf)
		# treeNode[0][-1].append(leaf)
		# treeNode[0][-1].append([leaf])
		# treeNode[-1].append([leaf])

	childNodes = getCallPathChildNodeIDs(nodeID, db)
	for childID in childNodes:
		buildCallPathNodeTraversal(runID, processID, db, leaf, childID, indentLevel + 1)

	if am_root:
		return treeNode


def getCallPathChildNodeIDs(callPathID, db):
	db.row_factory = sqlite3.Row
	cur = db.cursor()

	cmd = "SELECT CallPathID FROM CallPathData " + \
		  "WHERE ParentNodeID = " + str(callPathID)
	cur.execute(cmd)

	result = cur.fetchall()

	list = []
	for i in result:
		list.append(i['CallPathID'])

	return list

# do_plt_polar = True
do_plt_polar = False

do_bar_plot_vertical = True
# do_bar_plot_vertical = False

methodTypeToColour = {}
methodTypeToColour["Program"] = "silver"
methodTypeToColour["Method"] = "silver"
methodTypeToColour["Loop"] = "silver"
methodTypeToColour["ComputeLoop"] = "purple"
methodTypeToColour["MPICommCall"] = "blue"
methodTypeToColour["MPISyncCall"] = "orange"

def sunburst_root(nodes):
	if do_plt_polar:
		root_total = np.pi * 2
	else:
		label, methodType, value, subnodes = nodes[0]
		root_total = value
		print("root_total = {0}".format(root_total))

	if do_plt_polar:
		ax = plt.subplot(111, projection='polar')
	else:
		ax = plt.subplot(111)

	sunburst(nodes, root_total, root_total, 0, 0, ax)

# def sunburst(nodes, total=root_total, offset=0, level=0, ax=None):
def sunburst(nodes, root_total, node_total, offset, level, ax):
	if level == 0 and len(nodes) > 1:
		pprint(nodes)
		raise Exception("There must only be one root node, not {0}".format(len(nodes)))

	if level == 0 and len(nodes) == 1:
		label, methodType, value, subnodes = nodes[0]

		if do_plt_polar:
			ax.bar(x=[0], height=[0.5], width=[node_total], color=[methodTypeToColour[methodType]])
			ax.text(0, 0, label, ha='center', va='center')
		else:
			if do_bar_plot_vertical:
				ax.bar(x=[node_total/2], height=[1.0], width=[node_total], color=[methodTypeToColour[methodType]])
				ax.text(root_total/2, 0.5, label, ha='center', va='center')
			else:
				ax.bar(x=[0.5], height=[node_total], width=[1.0], color=[methodTypeToColour[methodType]])
				ax.text(0.5, root_total/2, label, ha='center', va='center')

		sunburst(subnodes, root_total, value, 0, level+1, ax)
	elif nodes:
		if do_plt_polar:
			d = root_total / node_total
		else:
			d = root_total / node_total
		sizes = []
		labels = []
		colours = []
		subnode_offset = offset
		# for label, value, subnodes in nodes:
		for label, methodType, value, subnodes in nodes:
			if (value / node_total) < 0.03:
				## Prune out small nodes
				continue
			labels.append(label)
			sizes.append(value * d)
			colours.append(methodTypeToColour[methodType])
			sunburst(subnodes, root_total, node_total, subnode_offset, level+1, ax)
			subnode_offset += value
		if do_plt_polar:
			widths = sizes
			heights = [1] * len(nodes)
			lefts = np.cumsum([offset * d] + widths[:-1])
			bottoms = np.zeros(len(nodes)) + level - 0.5
		else:
			if do_bar_plot_vertical:
				heights = [1] * len(sizes)
				widths = sizes
				bottoms = np.zeros(len(sizes)) + level
				lefts = np.cumsum([offset * d] + widths[:-1])
			else:
				widths = [1] * len(sizes)
				heights = sizes
				lefts = np.zeros(len(sizes)) + level
				bottoms = np.cumsum([offset * d] + heights[:-1])
		rects = ax.bar(x=lefts, height=heights, width=widths, bottom=bottoms, linewidth=1,
					   color=colours, edgecolor='white', align='edge')
		for rect, label in zip(rects, labels):
			x = rect.get_x() + rect.get_width() / 2
			y = rect.get_y() + rect.get_height() / 2
			if do_plt_polar:
				rotation = (90 + (360 - np.degrees(x) % 180)) % 360
				ax.text(x, y, label, rotation=rotation, ha='center', va='center')
			else:
				ax.text(x, y, label, ha='center', va='center')

	if level == 0:
		if do_plt_polar:
			ax.set_theta_direction(-1)
			ax.set_theta_zero_location('N')
			ax.set_axis_off()
		else:
			if do_bar_plot_vertical:
				ax.get_yaxis().set_visible(False)
			else:
				ax.get_xaxis().set_visible(False)

		## Build custom colour legend:
		legend_dict = { "MPI sync" : methodTypeToColour["MPISyncCall"],  "compute" : methodTypeToColour["ComputeLoop"] }
		patchList = []
		for k in legend_dict:
			patchList.append(mpatches.Patch(color=legend_dict[k], label=k))
		ax.legend(handles=patchList)


def printAggregateCallPathNodeDetails(callPathID, runID, processID, db):

	record = getAggregateCallPathNodeDetails(callPathID, runID, processID, db)

	print(str(record['Name']) + "," + \
		  str(record['TypeName']) + "," + \
		  str(record['AggMinTime']) + "," + \
		  str(record['AggAvgTime']) + "," + \
		  str(record['AggMaxTime']) + "," + \
		  str(record['CallCount']) + "," + \
		  str(record['AggTotalTimeI']) + "," + \
		  str(record['AggTotalTimeE']))

def getAggregateCallPathNodeDetails(callPathID, runID, processID, db):
	# This method retrieves the following aggregate details about a node and returns them as a dictionary
	# (1) Node Name: Key 'Name'
	# (2) Node Type Name: Key 'TypeName'
	# (3) Min Time: Key 'AggMinTime'
	# (4) Avg Time: Key 'AggAvgTime'
	# (5) Max Time: Key 'AggMaxTime'
	# (6) Count: Key 'CallCount'
	# (7) Total Time Inclusive (including child node times): Key 'AggTotalTimeI'
	# (8) Total Time Exclusive (time in this node only, excluding child node times): Key 'AggTotalTimeE'
	# (9)

	# ToDo: If looping over nodes, this will lead to the same data being loaded from the database multiple
	# times, which is likely expensive.
	# Options: Load full data from database into tree in memory (might be v. large)
	#		  Ensure only depth-first traversal that can reuse data from prior loads

	db.row_factory = sqlite3.Row
	cur = db.cursor()

	# Get Node Aggregate Details - Should only ever have one entry
	cmd = "SELECT C.NodeName AS NodeName, " + \
			"D.TypeName AS TypeName, " + \
			"A.MinWallTime AS MinTime, " + \
			"A.AvgWallTime AS AvgTime, " + \
			"A.MaxWallTime AS MaxTime, " + \
			"A.Count AS Count " + \
			"FROM AggregateTime AS A " + \
			"NATURAL JOIN CallPathData AS B " + \
			"NATURAL JOIN ProfileNodeData AS C " + \
			"NATURAL JOIN ProfileNodeType AS D " + \
			"WHERE A.CallPathID = " + str(callPathID) + " " + \
			"AND A.RunID = " + str(runID) + " " + \
			"AND A.ProcessID = " + str(processID) + \
		  	";"

	cur.execute(cmd)
	result = cur.fetchone()

	rDict = {}
	rDict['Name'] = result['NodeName']
	rDict['TypeName'] = result['TypeName']
	rDict['AggMinTime'] = result['MinTime']
	rDict['AggAvgTime'] = result['AvgTime']
	rDict['AggMaxTime'] = result['MaxTime']
	rDict['CallCount'] = result['Count']
	rDict['AggTotalTimeI'] = rDict['AggAvgTime'] * rDict['CallCount']

	# === Get Times for any direct children of this node ===
	childIDs = getCallPathChildNodeIDs(callPathID, db)

	if(len(childIDs) == 0):
		# No child nodes means exclusive time = inclusive time
		rDict['AggTotalTimeE'] = rDict['AggTotalTimeI']
	else:
		childIDStr = str(childIDs[0])

		# Build Comma separated list of ids
		if(len(childIDs) > 1):
			for id in childIDs:
				childIDStr = childIDStr + "," + str(id)

		# Get sum of inclusive times taken by child nodes
		cmd = "SELECT SUM(AvgWallTime * Count) AS ChildTimeSum FROM AggregateTime " + \
			  "WHERE RunID = " + str(processID) + " " + \
			  "AND CallPathID IN (" + str(childIDStr) + ")" + \
			  ";"

		cur.execute(cmd);
		result = cur.fetchone()

		rDict['AggTotalTimeE'] = rDict['AggTotalTimeI'] - result['ChildTimeSum']
		# Due to overhead, potentially possible for this to be slightly negative, so will correct here
		if(rDict['AggTotalTimeE'] < 0.0):
			rDict['AggTotalTimeE'] = 0.0

	return rDict

def printSummaryExclusiveTimeByType(runID, processID, db):
	rDict = getSummaryExclusiveTimeByType(runID, processID, db)

	print("==============================================")
	print("Summary: Exclusive Time in code blocks by type")
	print("==============================================")
	print("")

	print("Root Walltime: " + str(rDict['Root']))

	percentSum = 0.0

	for key in rDict:
		percent = (rDict[key]/rDict['Root'])*100
		percentSum = percentSum + percent
#		sys.stdout.write(str(rDict[key]))
		print(str(key) + "," + str(rDict[key]) + "," + str(round(percent,2)))

def getSummaryExclusiveTimeByType(runID, processID, db):

	# Get all callpath node IDs
	db.row_factory = sqlite3.Row
	cur = db.cursor()

	cmd = "SELECT DISTINCT CallPathID FROM AggregateTime NATURAL JOIN CallPathData WHERE RunID = " + str(runID) + ";"

	cur.execute(cmd)
	result = cur.fetchall()

	# For each callpath node, get the aggregate details
	nodeProp = []
	for row in result:
		nodeProp.append(getAggregateCallPathNodeDetails(row['CallPathID'], runID, processID, db))

	# Sort the list by the type name of each record
	sortedRecords = sorted(nodeProp, key=lambda k: k['TypeName'])

	# Get the time for the root node
	rootID = getRootCallPathID(db)
	rootProp = getAggregateCallPathNodeDetails(rootID, runID, processID, db)

	rDict = {}
	rDict['Root'] = rootProp['AggTotalTimeI']

	# Sum the exclusive times of nodes grouped by type
	group = sortedRecords[0]['TypeName']
	groupTime = 0.0
	percentsum = 0.0

	for record in sortedRecords:
		if(record['TypeName'] == group):
			groupTime = groupTime + record['AggTotalTimeE']
		else:
			percent = (groupTime/rootProp['AggTotalTimeI'])*100
			percentsum = percentsum + percent
			rDict[group] = groupTime
			group = record['TypeName']
			groupTime = record['AggTotalTimeE']

	# Last Group
	percent = (groupTime/rootProp['AggTotalTimeI'])*100
	percentsum = percentsum + percent
	rDict[group] = groupTime

	return rDict

if __name__ == "__main__":
	main()
