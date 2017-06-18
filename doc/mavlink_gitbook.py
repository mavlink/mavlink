#! /usr/bin/python

"""
This script generates a markdown file (message-definitions.md) that can be imported into a gitbook to display the MAVLink Common message format (HTML).
(this is a mechanism of bringing the current docs into the gitbook: http://mavlink.org/messages/common )

The script runs on both Python2 and Python 3. The following libraries must be imported: lxml, requests, bs4.

The file is generated using an XSLT transformation from the XML at "https://raw.github.com/mavlink/mavlink/master/message_definitions/v1.0/common.xml". 
"""

import lxml.etree as ET
import requests
from bs4 import BeautifulSoup as bs
import re

xml_file_url = "https://raw.github.com/mavlink/mavlink/master/message_definitions/v1.0/common.xml";
xsl_file_url= "https://raw.github.com/mavlink/mavlink/master/doc/mavlink_to_html_table_gitbook.xsl";
output_file_name = "./message-definitions.md"

# Get files from published URLs
r = requests.get(xml_file_url)
xml_file = r.text
r = requests.get(xsl_file_url)
xsl_file = r.text

dom = ET.fromstring(xml_file)
xslt = ET.fromstring(xsl_file)

transform = ET.XSLT(xslt)
newdom = transform(dom)

#Prettify the HTML using BeautifulSoup
soup=bs(str(newdom), "lxml")
prettyHTML=soup.prettify()


# Strip out all text before "<html>" - not needed in the output
def slicer(my_str,sub):
    index=my_str.find(sub)
    if index !=-1 :
        return my_str[index:] 
    else :
        return my_str
        
prettyHTML=slicer(prettyHTML,'<html>')

#Fix up the BeautifulSoup output so to fix build-link errors in the generated gitbook.
## BS puts each tag/content in its own line. Gitbook generates anchors using the spaces/newlines. 
## This puts displayed text content immediately within tags so that anchors/links generate properly
def fix_content_in_tags(input_html):
    #print("fix_content_in_tags was called")
    def remove_space_between_content_tags(matchobj):
        stripped_string=matchobj.group(1).strip()
        return '>%s<' % stripped_string

    input_html=re.sub(r'\>(\s+?\w+?.*?)\<', remove_space_between_content_tags, input_html,flags=re.DOTALL)
    return input_html
    
prettyHTML = fix_content_in_tags(prettyHTML)

    
    
with open(output_file_name, 'w') as out:
    out.write(prettyHTML )
    
print("COMPLETED")



