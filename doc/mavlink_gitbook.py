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
output_dir_html=output_dir+"_html/"
if not os.path.exists(output_dir_html):
    os.makedirs(output_dir_html)


# File for index
index_file_name = "README.md"
index_file_name = output_dir + index_file_name

# Get XSLT
with open(xsl_file_name, 'r') as content_file:
    xsl_file = content_file.read()
xslt = ET.fromstring(xsl_file)

#initialise text for index file. 
index_text="""<!-- THIS FILE IS AUTO-GENERATED (DO NOT UPDATE GITBOOK): https://github.com/mavlink/mavlink/blob/master/doc/mavlink_gitbook.py -->
# Dialects {#dialects}

MAVLink *dialects* are XML files that define *protocol-* and *vendor-specific* messages, enums and commands.

Dialects may *include* other MAVLink XML files.
A typical pattern is for a dialect to include [common.xml](../messages/common.md) (containing the *MAVLink standard definitions*), extending it with vendor or protocol specific messages.
While a dialect can include any other message definition, only only a single level of nesting is supported ([at time of writing](https://github.com/ArduPilot/pymavlink/pull/248)).

> **Note** Vendor forks of MAVLink may contain dialect messages that are not yet merged, and hence will not appear in this documentation.

The dialect files are stored alongside in separate XML files in [mavlink/message definitions](https://github.com/mavlink/mavlink/blob/master/message_definitions/).

The human-readable forms of the XML dialect files are linked below:
"""

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

def fix_replace_space_marker(input_html):
    ## Above we remove hidden space. I can't seem to regexp just that type of space, so use space markers in text
    input_html=input_html.replace('xxx_space_xxx',' ')
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
        insert_text+="""
# MAVLINK Common Message Set

The MAVLink *common* message set is defined in [common.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml).
It contains the *standard* definitions that are managed by the MAVLink project.

The definitions cover functionality that is considered useful to most ground control stations and autopilots.
MAVLink-compatible systems are expected to use these definitions where possible (if an appropriate message exists) rather than rolling out variants in their own [dialects](../messages/README.md).

This topic is a human-readable form of [common.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml).
"""
    elif filename == 'ardupilotmega.xml':
        insert_text+="""
# Dialect: ArduPilotMega

These messages define the ArduPilot specific message set, which is custom to [http://ardupilot.org](http://ardupilot.org).

This topic is a human-readable form of the XML definition file: [ardupilotmega.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml).

> **Warning** The ArduPilot MAVLink fork of [ardupilotmega.xml](https://github.com/ArduPilot/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml) may contain messages that have not yet been merged into this documentation.
"""
    else:
        insert_text+='\n# Dialect: %s' % filename.rsplit('.',1)[0]
        insert_text+='\n\n*This is a human-readable form of the XML definition file: [%s](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/%s).*' % (filename, filename)
    insert_text+="""

<span></span>
> **Note** MAVLink 2 messages have an ID > 255 and are marked up using **(MAVLink 2)** in their description.

<span id="mav2_extension_field"></span>
> **Note** MAVLink 2 extension fields that have been added to MAVLink 1 messages are displayed in blue.

<style>
td {
    vertical-align:top;
}
</style>
"""
    # Include HTML in generated content
    insert_text+='\n\n{%% include "_html/%s.html" %%}' % filename[:-4]
    input_html=insert_text+'\n\n'+input_html
    
    #print(input_html)
    return input_html
    
dialect_files = set()    

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
            
            #Replace invalid file extensions (workaround for xslt)
            prettyHTML = fix_include_file_extension(prettyHTML)

            #Replace space markers with intentional space
            prettyHTML = fix_replace_space_marker(prettyHTML)
            
            #Write output html file
            output_file_name_html = file.rsplit('.',1)[0]+".html"

            output_file_name_html_withdir = output_dir_html+output_file_name_html
            print("Output filename (html): %s" % output_file_name_html)

            with open(output_file_name_html_withdir, 'w') as out:
                out.write(prettyHTML)


            #Write output markdown file
            output_file_name_prefix = file.rsplit('.',1)[0]

            markdown_text=''
            #Inject a heading and doc-type intro (markdown format)
            markdown_text = inject_top_level_docs(markdown_text,file)

            output_file_name_md_withdir = output_dir+output_file_name_prefix+'.md'
            print("Output filename (md): %s" % output_file_name_md_withdir)

            with open(output_file_name_md_withdir, 'w') as out:
                out.write(markdown_text)

            # Create sortable list of output file names
            if not file=='common.xml':
                dialect_files.add(output_file_name_prefix)
            
for the_file in sorted(dialect_files):
    index_text+='\n* [%s.xml](%s.md)' % (the_file,the_file)
            
#Write the index
with open(index_file_name, 'w') as content_file:
    content_file.write(index_text)

print("COMPLETED")



