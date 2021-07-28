#!/bin/bash 
set -e

# Use  Serge Lamikhov's XSLT at https://gist.github.com/serge1/5521359 (in this dir as test2html.xsl)
PATH_TO_XSLT=$1

# Combine all results
rm -f combined-results.xml
cat report-*.xml | grep -v '<?xml version="1.0"?>'  > combined-results.xml
cat combined-results.xml | sed 's/<TestResult>//g' | sed 's/<\/TestResult>//g' > tmp.xml
echo '<?xml version="1.0"?>'  > combined-results.xml
echo "<TestResult>" >> combined-results.xml
cat tmp.xml >> combined-results.xml
echo "</TestResult>" >> combined-results.xml

rm -f combined-log.xml
cat log-*.xml | grep -v '<?xml version="1.0"?>'  > combined-log.xml
cat combined-log.xml | sed 's/<TestLog>//g' | sed 's/<\/TestLog>//g' > tmp.xml
echo '<?xml version="1.0"?>'  > combined-log.xml
echo "<TestLog>" >> combined-log.xml
cat tmp.xml >> combined-log.xml
echo "</TestLog>" >> combined-log.xml

rm tmp.xml

xsltproc $PATH_TO_XSLT combined-results.xml combined-log.xml > unittests.html