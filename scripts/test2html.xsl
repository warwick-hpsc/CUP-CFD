<?xml version="1.0"?>
<!-- From https://gist.github.com/serge1/5521359  by 
Serge Lamikhov-->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>

  <xsl:template match="/">
    <!--link rel="stylesheet" href="test2html.css" type="text/css" /-->
    <html>
      <style>
h1 {
    font-size:30px;
    margin:30;
    padding-top:30;
    padding-bottom:0;
    color:#699699;
}
h2 {
    font-size:20px;
    padding-top:20;
    color:#79a6a9;
}
body{
    font:11px/20px Georgia,"Times New Roman",Times,serif;
    margin:50;
    padding:0;
    background-color:#e0f0f0;
}
table {
    font-size:11px;
    color:#333333;
    border-width: 1px;
    border-color: #999999;
    border-collapse: collapse;
}
table th {
    background-color:#c3dde0;
    border-width: 1px;
    padding: 8px;
    border-style: solid;
    border-color: #a9c6c9;
}
table tr {
    background-color:#d4e3e5;
}
table td {
    border-width: 1px;
    padding: 8px;
    border-style: solid;
    border-color: #a9c6c9;
}
table td.errormsg {
    border-width: 1px;
    padding: 8px;
    border-style: solid;
    border-color: #a9c6c9;
    background-color: white;
    color: black;
    padding-left: 50
}
table td.failed {
    border-width: 1px;
    padding: 8px;
    border-style: solid;
    border-color: #a9c6c9;
    background-color: red;
    color: white
}
table td.passed {
    border-width: 1px;
    padding: 8px;
    border-style: solid;
    border-color: #a9c6c9;
    background-color: green;
  color: white
}
      </style>
      <body>
        <xsl:apply-templates select="TestResult" mode="Summary"/>
        <xsl:apply-templates select="TestLog"/>
        <xsl:apply-templates select="TestResult" mode="Results"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="TestResult|TestSuite" mode="Summary">
    <xsl:if test="local-name() = 'TestResult'">
        <h1>Test Summary</h1>
    </xsl:if>
    <table>
      <thead>
        <tr>
          <th rowspan="2">Suite Name</th>
          <th rowspan="2">Result</th>
          <th colspan="2">Test Cases</th>
          <th rowspan="2">Sub-Suites</th>
        </tr>
        <tr>
          <th>Passed</th>
          <th>Failed</th>
        </tr>
      </thead>
      <tbody>
        <xsl:for-each select="TestSuite">
          <tr>
            <td>
              <xsl:value-of select="@name"/>
            </td>
            <td class="{@result}">
              <xsl:value-of select="@result"/>
            </td>
            <td>
              <xsl:value-of select="@test_cases_passed"/>
            </td>
            <td>
              <xsl:value-of select="@test_cases_failed"/>
            </td>
            <td>
              <xsl:if test="count(TestSuite) != 0">
                <xsl:apply-templates select="." mode="Summary"/>
              </xsl:if>
            </td>
          </tr>
        </xsl:for-each>
      </tbody>
    </table>
  </xsl:template>

  <xsl:template match="TestResult" mode="Results">
    <h1>Test Results</h1>
    <xsl:apply-templates select="TestSuite" mode="Results"/>
  </xsl:template>
  
  <xsl:template match="TestSuite" mode="Results">
    <xsl:choose>
      <xsl:when test="count(TestSuite) = 0">
        <h2>
          <xsl:value-of select="@name" />
        </h2>
        <table>
          <thead>
            <tr>
              <th>Suite Name</th>
              <th>Result</th>
              <th>Assertions Passed</th>
              <th>Assertions Failed</th>
              <th>Expected Failures</th>
            </tr>
          </thead>
          <tbody>
            <xsl:for-each select="TestCase">
              <tr>
                <td>
                  <xsl:value-of select="@name"/>
                </td>
                <td class="{@result}">
                  <xsl:value-of select="@result"/>
                </td>
                <td>
                  <xsl:value-of select="@assertions_passed"/>
                </td>
                <td>
                  <xsl:value-of select="@assertions_failed"/>
                </td>
                <td>
                  <xsl:value-of select="@expected_failures"/>
                </td>
              </tr>
            </xsl:for-each>
          </tbody>
        </table>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="TestSuite" mode="Results"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template match="TestLog">
    <xsl:if test="count(Error) != 0">
      <h1>Test Log</h1>
      <table>
        <xsl:for-each select="Error">
          <tr>
            <td>
              <xsl:value-of select="@file"/> ( <xsl:value-of select="@line"/> )
            </td>
          </tr>
          <tr>
            <td class="errormsg">
              <xsl:value-of select="."/>
            </td>
          </tr>
        </xsl:for-each>
      </table>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>