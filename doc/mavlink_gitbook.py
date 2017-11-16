#! /usr/bin/python

"""
This script generates markdown files for all the MAVLink message definition XML at: 
https://github.com/mavlink/mavlink/tree/master/message_definitions/v1.0
  
The files can be imported into a gitbook to display the messages as HTML

The script runs on both Python2 and Python 3. The following libraries must be imported: lxml, requests, bs4.

The file is run in mavlink/doc/ with no arguments. It writes the files to /messages/
"""

import lxml.etree as ET
import requests
from bs4 import BeautifulSoup as bs
import re
import os # for walk


xsl_file_name = "mavlink_to_html_table_gitbook.xsl"
xml_message_definitions_dir_name = "../message_definitions/v1.0/"

output_dir = "./messages/"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# File for index
index_file_name = "README.md"
index_file_name = output_dir + index_file_name

# Get XSLT
with open(xsl_file_name, 'r') as content_file:
    xsl_file = content_file.read()
xslt = ET.fromstring(xsl_file)

#initialise text for index file. 
index_text='<!-- THIS FILE IS AUTO-GENERATED (DO NOT UPDATE GITBOOK): https://github.com/mavlink/mavlink/blob/master/doc/mavlink_gitbook.py -->'
index_text+='\n# Message Definitions'
index_text+='\n\nMAVLink messages are defined in XML files in the [mavlink/message definitions](https://github.com/mavlink/mavlink/blob/master/message_definitions/) folder. The messages that are common to all systems are defined in [common.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml) (only messages contained in this file are considered standard messages). MAVLink protocol-specific messages and vendor-specific messages are stored in separate XML files.'
index_text+='\n\nThe common messages are provided as human-readable tables in: [Common](../messages/common.md).'
index_text+='\n\n> **Note** Vendor forks of MAVLink may contain messages that are not get merged, and hence will not appear in this documentation.'
index_text+='\n\nThe human-readable forms of the vendor XML files are linked below:'


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
    
def fix_include_file_extension(input_html):
    ## Fixes up file extension .xml.md.unlikely (easier than fixing up the XSLT to strip file extensions!)
    input_html=input_html.replace('.xml.md.unlikely','.md')
    return input_html
    
def strip_text_before_string(original_text,strip_text):
    # Strip out all text before some string
    index=original_text.find(strip_text)
    stripped_string=original_text
    if index !=-1 :
        stripped_string = stripped_string[index:] 
    return stripped_string
    
def inject_top_level_docs(input_html,filename):
    #Inject top level heading and other details.
    print('FILENAME: %s' % filename)
    insert_text='<!-- THIS FILE IS AUTO-GENERATED: https://github.com/mavlink/mavlink/blob/master/doc/mavlink_gitbook.py -->'
    if filename == 'common.xml':
        insert_text+='\n# MAVLINK Common Message Set'
        insert_text+='\n\nThese messages define the common message set, which is the reference message set implemented by most ground control stations and autopilots.'
        insert_text+='\n\n*This is a human-readable form of the XML definition file: [common.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml).*'
    elif filename == 'ardupilotmega.xml':
        insert_text+='\n# MAVLINK ArduPilotMega Message Set'
        insert_text+='\n\nThese messages define the APM specific message set, which is custom to [http://ardupilot.org](http://ardupilot.org).'
        insert_text+='\n\n*This is a human-readable form of the XML definition file: [ardupilotmega.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml).*'
        insert_text+='\n\n> **Warning** The ArduPilot MAVLink fork of [ardupilotmega.xml](https://github.com/ArduPilot/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml) may contain messages that have not yet been merged into this documentation.'
    else:
        insert_text+='\n# MAVLINK Message Set: %s' % filename
        insert_text+='\n\n*This is a human-readable form of the XML definition file: [%s](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/%s).*' % (filename, filename)


    insert_text+='\n\n<span></span>\n> **Note** MAVLink 2 messages have an ID > 255 and are marked up using **(MAVLink 2)** in their description.'
    insert_text+='\n\n<span id="mav2_extension_field"></span>\n> **Note** MAVLink 2 extension fields that have been added to MAVLink 1 messages are displayed in blue.'
    input_html=insert_text+'\n\n'+input_html
    #print(input_html)
    return input_html
    
    

for subdir, dirs, files in os.walk(xml_message_definitions_dir_name):
    for file in files:
        print(file)
        if not file.endswith('.xml'): #only process xml files.
           continue
        xml_file_name = xml_message_definitions_dir_name+file
        with open(xml_file_name, 'r') as content_file:
            xml_file = content_file.read()
            dom = ET.fromstring(xml_file)
            transform = ET.XSLT(xslt)
            newdom = transform(dom)

            #Prettify the HTML using BeautifulSoup
            soup=bs(str(newdom), "lxml")
            prettyHTML=soup.prettify()

            #Strip out text before <html> tag in XSLT output
            prettyHTML=strip_text_before_string(prettyHTML,'<html>')
            prettyHTML = fix_content_in_tags(prettyHTML)
            
            #Inject a heading and doc-type intro (markdown format)
            prettyHTML = inject_top_level_docs(prettyHTML,file)
            
            #Replace invalid file extensions (workaround for xslt)
            prettyHTML = fix_include_file_extension(prettyHTML)
            
            #Write output markdown file
            output_file_name = file.rsplit('.',1)[0]+".md"
            output_file_name_withdir = output_dir+output_file_name
            print("Output filename: %s" % output_file_name)

            with open(output_file_name_withdir, 'w') as out:
                out.write(prettyHTML )
            
            if not file=='common.xml':
                index_text+='\n* [%s](%s)' % (file,output_file_name)
            
#Write the index - Disabled for now.
#with open(index_file_name, 'w') as content_file:
#    content_file.write(index_text)


print("COMPLETED")



