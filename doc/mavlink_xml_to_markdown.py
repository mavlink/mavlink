#! /usr/bin/python

"""
This script generates markdown files for the MAVLink message definition XML at:
https://github.com/mavlink/mavlink/tree/master/message_definitions/v1.0

The files can be imported into a markdown SSG to display the messages as HTML

The script runs on Python 3.
The following libraries must be imported:
- bs4.
- lxml

The file is run in mavlink/doc/ with no arguments.
It writes the files to /messages/
It can also be run for a specific dialect, if specified.
It can also be imported and used to get information about the XML.
"""

# import lxml.etree as ET
# import requests
from bs4 import BeautifulSoup as bs
import re
import os  # for walk

import argparse  # for command line parsing

MAXIMUM_INCLUDE_FILE_NESTING = 5


class MAVXML(object):
    '''Represents a MAVLink XML file'''

    def __init__(self, filename):
        self.filename = filename
        self.basename = os.path.basename(filename)
        if self.basename.lower().endswith(".xml"):
            self.basename = self.basename[:-4]
        self.basename_upper = self.basename.upper()
        self.messages = {}  # dict
        self.enums = {}
        self.commands = {}
        self.includes = []
        self.dialect = None
        self.version = None

        # Read the XML file
        with open(self.filename, 'r') as f:
            xml_content = f.read()
        # Initialize BeautifulSoup with the XML content
        soup = bs(xml_content, 'xml')

        # Extract dialect
        dialect = soup.find('dialect')
        if dialect:
            self.dialect = dialect.text

        # Extract version
        version = soup.find('version')
        if version:
            self.version = version.text

        # Extract includes
        includes = soup.find_all('include')
        for include in includes:
            self.includes.append(include.text[:-4])

        # Extract and reorder messages
        messages = soup.find_all('message')
        for message in messages:
            item = MAVMessage(message, self.basename)
            self.messages[item.name] = item
        # reorder messages by id
        sorted_items = sorted(self.messages.items(),
                              key=lambda item: item[1].id)
        # Clear the original dictionary and rebuild it with the sorted items
        self.messages.clear()
        self.messages.update(sorted_items)

        # Extact all ENUM except MAV_CMD
        # Define a custom filter function to exclude "MAV_CMD"
        def exclude_mav_cmd(tag):
            return tag.name == 'enum' and tag.get('name') != 'MAV_CMD'
        filtered_enums = soup.find_all(exclude_mav_cmd)
        for enum in filtered_enums:
            # print(f"debug: enumTestDalect: {self.basename}")
            item = MAVEnum(enum, self.basename)
            self.enums[item.name] = item
        # reorder the enum values
        for enumName in self.enums.keys():
            # reorder the enum values - sort the entries based on the 'value' property
            mav_enum_entries = self.enums[enumName].entries.values()
            sorted_entries = sorted(
                mav_enum_entries, key=lambda entry: entry.value)
            # Create a new dictionary with the sorted entries
            sorted_enum_entries = {
                entry.name: entry for entry in sorted_entries}
            # Clear the original dictionary and rebuild it with the sorted items
            self.enums[enumName].entries.clear()
            self.enums[enumName].entries.update(sorted_enum_entries)

        # Extract Commands (MAV_CMD) and reorder
        mav_cmd_enum = soup.find('enum', attrs={'name': 'MAV_CMD'})
        if mav_cmd_enum:
            mav_commands = mav_cmd_enum.find_all('entry')
            for command in mav_commands:
                item = MAVCommand(command, self.basename)
                self.commands[item.name] = item
        # reorder commands by id
        # Sort the items of the dictionary based on the id property of the value (second element)
        sorted_items = sorted(self.commands.items(),
                              key=lambda item: item[1].value)
        # Clear the original dictionary and rebuild it with the sorted items
        self.commands.clear()
        self.commands.update(sorted_items)

    def mergeIn(self, mergeXML):
        """Merge a passed file into this file"""
        # print(f"debug: mergeIn {mergeXML.basename} into {self.basename}")

        # merge messages
        for messageName in mergeXML.messages.keys():
            if messageName in self.messages:
                # print(f"debug: mergeIn {messageName} already present, skip")
                continue
            else:
                # print(f"debug: mergeIn {messageName} added from {mergeXML.basename}")
                self.messages[messageName] = mergeXML.messages[messageName]
        # reorder messages by id
        # Sort the items of the dictionary based on the id property of the value (second element)
        sorted_items = sorted(self.messages.items(),
                              key=lambda item: item[1].id)
        # Clear the original dictionary and rebuild it with the sorted items
        self.messages.clear()
        self.messages.update(sorted_items)

        # merge commands
        for commandName in mergeXML.commands.keys():
            if commandName in self.commands:
                # print(f"debug: mergeIn {commandName} already present, skip")
                continue
            else:
                # print(f"debug: mergeIn {commandName} added from {mergeXML.basename}")
                self.commands[commandName] = mergeXML.commands[commandName]
        # reorder commands by id now imported
        # Sort the items of the dictionary based on the id property of the value (second element)
        sorted_items = sorted(self.commands.items(),
                              key=lambda item: item[1].value)
        # Clear the original dictionary and rebuild it with the sorted items
        self.commands.clear()
        self.commands.update(sorted_items)

        # merge enums
        for enumName in mergeXML.enums.keys():
            if enumName in self.enums:
                # print(f"TODO need to merge the values: debug: mergeIn {enumName} already present, skip")
                for enumValue in mergeXML.enums[enumName].entries.keys():

                    if enumValue in self.enums[enumName].entries:
                        # print(f"{enumValue} - skip: already present")
                        pass
                    else:
                        # add value from lower level that hasn't been replaced
                        self.enums[enumName].entries[enumValue] = mergeXML.enums[enumName].entries[enumValue]

                # reorder the enum values now imported - sort the entries based on the 'value' property
                mav_enum_entries = self.enums[enumName].entries.values()
                sorted_entries = sorted(
                    mav_enum_entries, key=lambda entry: entry.value)
                # Create a new dictionary with the sorted entries
                sorted_enum_entries = {
                    entry.name: entry for entry in sorted_entries}
                # Clear the original dictionary and rebuild it with the sorted items
                self.enums[enumName].entries.clear()
                self.enums[enumName].entries.update(sorted_enum_entries)
                # continue
            else:
                # print(f"debug: mergeIn {enumName} added from {mergeXML.basename}")
                # Enum is new, so just merge it
                self.enums[enumName] = mergeXML.enums[enumName]

    def getMarkdown(self):
        """Generate Markdown for this XML file"""
        # markdownText = f"# {self.basename}\n\n"
        markdownText = ""

        intro_text = self.get_top_level_docs(self.basename)
        markdownText += intro_text

        # Generate dialect and version if present
        if self.dialect:
            markdownText += f"**Protocol dialect:** {self.dialect}\n\n"
        if self.version:
            markdownText += f"**Protocol version:** {self.version}\n\n"

        # Generate include files docs
        markdownText += "## MAVLink Include Files\n\n"
        if self.includes:
            base_path = '../messages/'
            # Create a list of formatted strings
            for include in self.includes:
                # markdownText+="\n"
                markdownText += f"- [{include}.xml]({base_path}{include}.md)\n"
        else:
            markdownText += " None\n"
        markdownText += "\n"

        # Get counts of entities
        entity_summary = "## Summary\n\n"
        entity_summary += "Type | Defined | Included\n"
        entity_summary += "--- | --- | ---\n"


        matching_count = 0
        non_matching_count = 0
        for message in self.messages.values():
            if message.basename == self.basename:
                matching_count += 1
            else:
                non_matching_count += 1
        result_string = result_string = f"{'[Messages](#messages)' if matching_count + non_matching_count > 0 else 'Messages'} | {matching_count} | {non_matching_count}\n"
        entity_summary += result_string

        matching_count = 0
        non_matching_count = 0
        for enum in self.enums.values():
            if enum.basename == self.basename:
                matching_count += 1
            else:
                non_matching_count += 1
        result_string = result_string = f"{'[Enums](#enumerated-types)' if matching_count + non_matching_count > 0 else 'Enums'} | {matching_count} | {non_matching_count}\n"
        entity_summary += result_string

        matching_count = 0
        non_matching_count = 0
        for commands in self.commands.values():
            if commands.basename == commands.basename:
                matching_count += 1
            else:
                non_matching_count += 1
        result_string = result_string = f"{'[Commands](#mav_commands)' if matching_count + non_matching_count > 0 else 'Commands'} | {matching_count} | {non_matching_count}\n\n"
        entity_summary += result_string

        entity_summary += "The following sections list all entities in the dialect (both included and defined in this file).\n\n"
        markdownText += entity_summary

        if len(self.messages):
            markdownText += "## Messages\n\n"
        for message in self.messages.values():
            # Get markdown assuming base dialect of this XML
            markdownText += message.getMarkdown(self.basename)

        if len(self.enums):
            markdownText += "## Enumerated Types\n\n"
        for enum in self.enums.values():
            markdownText += enum.getMarkdown(self.basename)

        if len(self.commands):
            markdownText += "## Commands (MAV_CMD) {#mav_commands}\n\n"
        for command in self.commands.values():
            markdownText += command.getMarkdown(self.basename)

        return markdownText

    def get_top_level_docs(self, filename):
        # Inject top level heading and other details.
        # print('FILENAME (prefix): %s' % filename)
        insert_text = '<!-- THIS FILE IS AUTO-GENERATED: https://github.com/mavlink/mavlink/blob/master/doc/mavlink_xml_to_markdown.py -->\n\n'
        if filename == 'common':
            insert_text += """
# MAVLINK Common Message Set (common.xml)

The MAVLink *common* message set contains *standard* definitions that are managed by the MAVLink project.
The definitions cover functionality that is considered useful to most ground control stations and autopilots.
MAVLink-compatible systems are expected to use these definitions where possible (if an appropriate message exists) rather than rolling out variants in their own [dialects](../messages/README.md).

The original definitions are defined in [common.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml).
"""
        elif filename == 'minimal':
            insert_text += """
# MAVLink Minimal Set (minimal.xml)

The MAVLink *minimal* set contains the minimal set of definitions for a viable MAVLink system.

The message set is defined in [minimal.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/minimal.xml) and is managed by the MAVLink project.

> **Tip** The minimal set is included (imported into) other xml definition files, including the [MAVLink Common Message Set (common.xml)](minimal.md).

"""
        elif filename == 'standard':
            insert_text += """
# Dialect: MAVLINK Standard Message Set (standard.xml)

The MAVLink *standard* message set contains *standard* definitions that are managed by the MAVLink project.
The definitions are those that are expected to be implemented in all flight stacks/ground stations
AND are likely to be implemented in a compatible way.
The original definitions are defined in [standard.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/standard.xml).
    """

        elif filename == 'development':
            insert_text += """
# Dialect: development

This dialect contains messages that are proposed for inclusion in the [standard set](standard.md), in order to ease development of prototype implementations.
They should be considered a 'work in progress' and not included in production builds.

This topic is a human-readable form of the XML definition file: [development.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/development.xml).
"""
        elif filename == 'test':
            insert_text += """
# Dialect: test

The test dialect is used for testing XML file parsing.

This topic is a human-readable form of the XML definition file: [test.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/test.xml).
"""
        elif filename == 'all':
            insert_text += """
# Dialect: all

This dialect is intended to `include` all other [dialects](../messages/README.md) in the [mavlink/mavlink](https://github.com/mavlink/mavlink) repository (including [external dialects](https://github.com/mavlink/mavlink/tree/master/external/dialects#mavlink-external-dialects)).

Dialects that are in **all.xml** are guaranteed to not have clashes in messages, enums, enum ids, and MAV_CMDs.
This ensure that:

- Systems based on these dialects can co-exist on the same MAVLink network.
- A Ground Station might (optionally) use libraries generated from **all.xml** to communicate using any of the dialects.

> **Warning**
>
> - New dialect files in the official repository must be added to **all.xml** and restrict themselves to using ids in their own allocated range.
> - Dialects should push changes to mavlink/mavlink in order to avoid potential clashes from changes to other dialects.
>
> A few older dialects are not included because these operate in completely closed networks or because they are only used for tests.

This topic is a human-readable form of the XML definition file: [all.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/all.xml).
"""

        elif filename == 'ardupilotmega':
            insert_text += """
# Dialect: ArduPilotMega

> **Warning** [ardupilotmega.xml](https://github.com/ArduPilot/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml) contains the accurate and up-to-date documentation for this dialect.
> The documentation below may not be accurate if the dialect owner has not pushed changes.

These messages define the [ArduPilot](http://ardupilot.org) specific dialect (as pushed to the [mavlink/mavlink](https://github.com/mavlink/mavlink) GitHub repository by the dialect owner).

This topic is a human-readable form of the XML definition file: [ardupilotmega.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/ardupilotmega.xml).

    """

        elif filename == 'cubepilot':
            insert_text += """
# Dialect: cubepilot

> **Warning** [cubepilot.xml](https://github.com/CubePilot/mavlink/blob/master/message_definitions/v1.0/cubepilot.xml) contains the accurate and up-to-date documentation for this dialect.
> The documentation below may not be accurate if the dialect owner has not pushed changes.

These messages define the [CubePilot](http://www.cubepilot.com) specific dialect (as pushed to the [mavlink/mavlink](https://github.com/mavlink/mavlink) GitHub repository by the dialect owner).

This topic is a human-readable form of the XML definition file: [cubepilot.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/cubepilot.xml).

    """


        else:
            dialectName = filename.rsplit('.', 1)[0]
            filenameXML = f'{dialectName}.xml'
            insert_text += f"""
# Dialect: {dialectName}

> **Warning** This topic documents the version of the dialect file in the [mavlink/mavlink](https://github.com/mavlink/mavlink) Github repository, which may not be up to date with the file in the source repository (it is up to the dialect owner to push changes when needed).
> The source repo should be listed in the comments at the top of the XML definition file listed below (but may not be).

This topic is a human-readable form of the XML definition file: [{filenameXML}](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/{filenameXML}).
"""
        insert_text += """

<span id="mav2_extension_field"></span>

> **Note**
> - MAVLink 2 [extension fields](../guide/define_xml_element.md#message_extensions) are displayed in blue.
> - Entities from dialects are displayed only as headings (with link to original)

<style>
span.ext {
    color: blue;
  }
span.warning {
    color: red;
  }
</style>
"""

        return insert_text

class MAVDeprecated(object):
    def __init__(self, soup):
        # name, type, print_format, xml, description='', enum='', display='', units='', instance=False
        self.since = soup.get('since')
        self.replaced_by = soup.get('replaced_by')
        self.description = soup.text
        if self.description:
            self.description = fix_add_implicit_links_items(self.description)

        # self.debug()

    def getMarkdown(self):
        markdown = "**DEPRECATED:**"
        markdown += f" Replaced By {fix_add_implicit_links_items(self.replaced_by)} " if self.replaced_by else ''
        markdown += f"({self.since})" if self.since else ''
        markdown += f" — {self.description})" if self.description else ''
        markdown = f'<span class="warning">{markdown.strip()}</span>'
        return markdown

    def debug(self):
        print(
            f"debug:Deprecated: since({self.since}), replaced_by({fix_add_implicit_links_items(self.replaced_by)}), description({self.description})")


class MAVWip(object):
    def __init__(self, soup=None):
        # <wip/>
        # self.wip = True
        self.description = None
        if soup:
            self.description = soup.text

        # self.debug()

    def getMarkdown(self):
        if self.description:
            print(f"TODO: MAVWIP: desc not printed: {self.name}")
        markdown = '**WORK IN PROGRESS**: Do not use in stable production environments (it may change).'
        markdown = f'<span class="warning">{markdown.strip()}</span>'
        return markdown

    def debug(self):
        print(f"debug:MAVWip: desc({self.description})")


class MAVField(object):
    def __init__(self, soup, parent, extension):
        # name, type, print_format, xml, description='', enum='', display='', units='', instance=False
        self.name = None
        self.type = None
        self.units = None
        self.enum = None
        self.display = None
        self.instance = None
        self.print_format = None
        self.invalid = None
        self.default = None
        self.minValue = None
        self.maxValue = None
        self.multiplier = None
        self.extension = extension
        for attr, value in soup.attrs.items():
            # We do it this way to catch all of them. New additions will throw debug
            if attr == 'name':
                self.name = value
            elif attr == 'type':
                self.type = value
            elif attr == 'units':
                self.units = value
            elif attr == 'enum':
                self.enum = value
            elif attr == 'display':
                self.display = value
            elif attr == 'instance':
                self.instance = True
            elif attr == 'print_format':
                self.print_format = value
            elif attr == 'invalid':
                self.invalid = value
            elif attr == 'default':
                self.default = value
            elif attr == 'minValue':
                self.minValue = value
            elif attr == 'maxValue':
                self.maxValue = value
            elif attr == 'multiplier':
                self.multiplier = value
            else:
                print(
                    f"Debug: MAVField: Unexpected attribute: {attr}, Value: {value}")

        # self.name_upper = self.name.upper()
        self.description = soup.contents  # may need further processing
        if not self.description:
            self.description = None
            # print(f"DEBUG: field desc not defined: {self.name}")
        elif len(self.description) == 1:
            self.description = self.description[0]  # Expected
        else:
            print(
                f"DEBUG: field desc multiple array problem: {self.name} (len: {len(self.description)} )")
            for item in self.description:
                print(f"  DEBUG: {item}")

        # Tell the message what field types it has - needed for table rendering
        # parent.fieldnames.add('name')
        # parent.fieldnames.add('type')
        parent.fieldnames.add('description')
        if self.units:
            parent.fieldnames.add('units')
        if self.enum:
            parent.fieldnames.add('enum')
        if self.display:
            parent.fieldnames.add('display')
        if self.print_format:
            parent.fieldnames.add('print_format')
        if self.instance:
            parent.fieldnames.add('instance')
        if self.minValue:
            parent.fieldnames.add('minValue')
        if self.maxValue:
            parent.fieldnames.add('minValue')
        if self.default:
            parent.fieldnames.add('default')
        if self.invalid:
            parent.fieldnames.add('invalid')
        if self.multiplier:
            parent.fieldnames.add('multiplier')
        # self.debug()

    def debug(self):
        print(
            f"Debug_Field- name ({self.name}), type ({self.type}), desc({self.description}), units({self.units}), display({self.display}), instance({self.instance}), multiplier({self.multiplier})")
        # TODO - display, instance, are not output.


class MAVMessage(object):
    def __init__(self, soup, basename):
        self.name = soup['name']
        self.id = int(soup['id'])
        self.name_lower = self.name.lower()
        self.basename = basename
        # self.linenumber = linenumber

        self.deprecated = None
        self.wip = None
        self.fields = []
        self.fieldnames = set()
        if self.basename == 'development':
            self.wip = MAVWip()

        # iterate the fields of our message
        extension = None
        for child in soup.children:
            if child.name:  # Check if the child is a tag (not a text node)
                if child.name == 'extensions':
                    extension = True
                elif child.name == 'field':
                    self.fields.append(MAVField(child, self, extension))
                elif child.name == 'description':
                    # Will do more processing this.
                    self.description = child.contents
                    if len(self.description) == 1:
                        self.description = self.description[0]
                        self.description = tidyDescription(self.description)
                        self.description = fix_add_implicit_links_items(
                            self.description)
                    else:
                        print(
                            f"DEBUG: message desc multiple array problem: {self.name}")
                    pass
                elif child.name == 'deprecated':
                    self.deprecated = MAVDeprecated(child)
                elif child.name == 'wip':
                    self.wip = MAVWip(child)
                else:
                    print(f"MAVMessage: Unexpected tag: {child.name}")

        # fields = soup.find_all('field')
        # for field in fields:
        #    self.fields.append(MAVField(field, self))

        # self.debug()

    def getMarkdown(self, currentDialect):
        """
        Return markdown for a message.
        """
        message = f"### {self.name} ({self.id})"

        # Add marker after name if there are additions
        if self.basename is not currentDialect or self.deprecated or self.wip:
            message += " —"

        # From dialect to heading if in dialect
        if self.basename is not currentDialect:
            # With basename (dialect name) test
            message += f" \[from: [{self.basename}](../messages/{self.basename}.md#{self.name})\]"

        if self.deprecated:
            message += " [DEP]"
        elif self.wip:
            message += " [WIP]"
            # message+=f"Included from [{self.basename}](../messages/{self.basename}.md#{self.name})\n\n"  # With basename (dialect name) test
        message += ' {#' + self.name + '}\n\n'

        # If it is common we include everything.
        # But for any other dialect we don't include the reset of the message
        if currentDialect == 'common':
            pass
        elif self.basename is not currentDialect:
            return message

        if self.deprecated:
            message += self.deprecated.getMarkdown()+"\n\n"
        if self.wip:
            message += self.wip.getMarkdown()+"\n\n"

        message += self.description + '\n\n'
        # message+=self.description + f" ({self.basename})\n\n"  # With dialect test

        # Test code for building this using the table builder
        # Note, might need to modify for new max/min stuff
        tableHeadings = []
        tableHeadings.append('Field Name')
        tableHeadings.append('Type')
        valueHeading = False
        unitsHeading = False
        multiplierHeading = False
        if any(field in self.fieldnames for field in ('units',)):
            unitsHeading = True
            tableHeadings.append('Units')
        if any(field in self.fieldnames for field in ('multiplier',)):
            multiplierHeading = True
            tableHeadings.append('Multiplier')
        if any(field in self.fieldnames for field in ('enum', 'invalid, maxValue, minValue, default')):
            valueHeading = True
            tableHeadings.append('Values')
        tableHeadings.append('Description')

        tableRows = []
        for field in self.fields:
            row = []
            nameText = f"<span class='ext'>{field.name}</span> <a href='#mav2_extension_field'>++</a>" if field.extension else f"{field.name}"
            row.append(nameText)
            row.append(f"`{field.type}`")
            if unitsHeading:
                row.append(f"{field.units if field.units else ''}")
            if multiplierHeading:
                row.append(f"{field.multiplier if field.multiplier else ''}")
            if valueHeading:
                # Values: #invalid, default, minValue, maxValue.
                values = []
                invalidText = f'invalid:{field.invalid}' if field.invalid else ''
                values.append(invalidText)
                defaultText = f'default:{field.default}' if field.default else ''
                values.append(defaultText)
                minValueText = f'min:{field.minValue}' if field.minValue else ''
                values.append(minValueText)
                maxValueText = f'max:{field.maxValue}' if field.maxValue else ''
                values.append(maxValueText)

                enumText = f"{fix_add_implicit_links_items(field.enum) if field.enum else ''}"
                values.append(enumText)

                # single elements only get one space
                valueText = "".join(
                    f"{elem} " if elem else "" for elem in values)
                row.append(valueText.strip())

            descriptionText = f"{fix_add_implicit_links_items(tidyDescription(field.description,'table'))}" if field.description else ''
            instanceText = '<br>Messages with same value are from the same source (instance).' if field.instance else ''
            descriptionText += instanceText
            row.append(descriptionText.strip())
            tableRows.append(row)

        # print("debugtablerows")
        # print(tableRows)

        message += generateMarkdownTable(tableHeadings, tableRows)
        message += "\n\n"
        return message

    def debug(self):
        print(
            f"debug:message: name({self.name}, id({self.id}), description({self.description}), deprecated({self.deprecated})")


class MAVEnumEntry(object):
    def __init__(self, soup, basename):
        # name, value, description='', end_marker=False, autovalue=False, origin_file='', origin_line=0, has_location=False
        self.name = soup['name']
        self.value = int(soup.get('value')) if soup.get('value') else print(
            f"TODO MISSING VALUE in MAVEnumEntry: {self.name}")
        self.basename = basename
        self.description = soup.findChild('description', recursive=False)
        self.description = self.description.text if self.description else None
        self.deprecated = soup.findChild('deprecated', recursive=False)
        self.deprecated = MAVDeprecated(
            self.deprecated) if self.deprecated else None
        self.wip = soup.findChild('wip', recursive=False)
        self.wip = MAVWip(self.wip) if self.wip else None
        # self.autovalue = autovalue  # True if value was *not* specified in XML

    def getMarkdown(self, currentDialect):
        """Return markdown for an enum entry"""
        deprString = f"<b>{self.deprecated.getMarkdown()}" if self.deprecated else ""
        if self.wip:
            print(f"TODO: WIP in Enum Entry: {self.name}")
        importedNote = ""
        if self.basename is not currentDialect:
            importedNote = " — \[from: [{self.basename}](../messages/{self.basename}.md#{self.name})\]"
        if self.basename is not currentDialect:
            print(
                f"TODO/Debug: Check rendering - imported merged enum value {self.name}")
        desc = fix_add_implicit_links_items(tidyDescription(
            self.description, 'table')) if self.description else ""
        string = f"<a id='{self.name}'></a>{self.value} | [{self.name}](#{self.name}) | {desc}{importedNote}{deprString} \n"
        return string


class MAVEnum(object):
    def __init__(self, soup, basename):
        # name, linenumber, description='', bitmask=False
        self.basename = basename  # dialect declared in
        self.name = None
        self.bitmask = None
        self.entries = {}

        for attr, value in soup.attrs.items():
            if attr == 'name':
                self.name = value
            elif attr == 'bitmask':
                self.bitmask = True
            else:
                print(
                    f"Debug: MAVEnum: Unexpected attribute: {attr}, Value: {value}")

        self.description = soup.findChild('description', recursive=False)
        self.description = tidyDescription(
            self.description.text) if self.description else None
        self.deprecated = soup.findChild('deprecated', recursive=False)
        self.deprecated = MAVDeprecated(
            self.deprecated) if self.deprecated else None
        if self.basename == 'development':
            self.wip = MAVWip()
        else:
            self.wip = soup.findChild('wip', recursive=False)
            self.wip = MAVWip(self.wip) if self.wip else None
        self.bitmask = soup.get('bitmask')
        enumEntries = soup.find_all('entry')
        for entry in enumEntries:
            enumVal = MAVEnumEntry(entry, self.basename)
            self.entries[enumVal.name] = enumVal

        # self.debug()

    def getMarkdown(self, currentDialect):
        """Return markdown for a whole enum"""

        string = f"### {self.name}"

        # Add marker after name if there are additions
        if self.basename is not currentDialect or self.deprecated or self.wip:
            string += " —"

        if self.basename is not currentDialect:
            # With basename (dialect name) test
            string += f" \[from: [{self.basename}](../messages/{self.basename}.md#{self.name})\]"

        if self.deprecated:
            string += " [DEP]"
        elif self.wip:
            string += " [WIP]"
            # message+=f"Included from [{self.basename}](../messages/{self.basename}.md#{self.name})\n\n"  # With basename (dialect name) test
        string += ' {#' + self.name + '}\n\n'

        # If it is common we include everything.
        # But for any other dialect we don't include the reset of the message
        if currentDialect == 'common':
            pass
        elif self.basename is not currentDialect:
            return string

        if self.deprecated:
            string += self.deprecated.getMarkdown()+"\n\n"

        if self.wip:
            string += self.wip.getMarkdown() + "\n\n"

        # if self.name=="MAV_FRAME":
        #    pass
        #    self.debug()

        string += "(Bitmask) " if self.bitmask else ""
        string += f"{fix_add_implicit_links_items(self.description)}" if self.description else ""
        if self.bitmask or self.description:
            string += "\n\n"
        string += "Value | Name | Description\n--- | --- | ---\n"
        for entry in self.entries.values():
            string += entry.getMarkdown(self.basename)
        string += "\n"

        return string

    def debug(self):
        print(
            f"debug:MAVEnum: name({self.name}), bitmask({self.bitmask}), deprecated({self.deprecated}), wip({self.wip}), basename({self.basename})")


class MAVCommandParam(object):
    def __init__(self, soup, parent):
        # name, value, description='', end_marker=False, autovalue=False, origin_file='', origin_line=0, has_location=False

        self.index = None
        self.label = None
        self.units = None
        self.minValue = None
        self.maxValue = None
        self.increment = None
        self.enum = None
        self.description = None
        self.reserved = None
        self.default = None
        self.multiplier = None

        for attr, value in soup.attrs.items():
            # We do it this way to catch all of them. New additions will throw debug
            if attr == 'index':
                self.index = int(value)
            elif attr == 'label':
                self.label = value
            elif attr == 'units':
                self.units = value
            elif attr == 'minValue':
                self.minValue = value
            elif attr == 'maxValue':
                self.maxValue = value
            elif attr == 'enum':
                self.enum = value
            elif attr == 'increment':
                self.increment = value
            elif attr == 'reserved':
                self.reserved = True  # TODO is it ever reserved by default, and if so make happen
            elif attr == 'default':
                self.default = value  # TODO is it ever default by default, and if so make happen?
            elif attr == 'multiplier':
                self.multiplier = value
            else:
                print(
                    f"Debug: MAVCommandParam: Unexpected attribute: {attr}, Value: {value}")

        if soup.text:
            self.description = soup.text
        if self.description:
            self.description = tidyDescription(self.description, "table")
        # no deprecated or wip supported
        # self.autovalue = autovalue  # True if value was *not* specified in XML

        # Add fields to display in parent.
        parent.param_fieldnames.add('index')
        if self.label:
            parent.param_fieldnames.add('label')
        if self.units:
            parent.param_fieldnames.add('units')
        if self.minValue:
            parent.param_fieldnames.add('minValue')
        if self.maxValue:
            parent.param_fieldnames.add('maxValue')
        if self.increment:
            parent.param_fieldnames.add('increment')
        if self.enum:
            parent.param_fieldnames.add('enum')
        if self.multiplier:
            parent.param_fieldnames.add('multiplier')

class MAVCommand(object):
    def __init__(self, soup, basename):
        # name, value, description='', end_marker=False, autovalue=False, origin_file='', origin_line=0, has_location=False
        pass
        self.name = soup['name']
        self.value = int(soup.get('value')) if soup.get(
            'value') else "TODO MISSING VALUE"
        self.basename = basename
        self.description = soup.description.text if soup.description else None
        if self.description:
            self.description = tidyDescription(self.description)
        self.deprecated = soup.findChild('deprecated', recursive=False)
        self.deprecated = MAVDeprecated(
            self.deprecated) if self.deprecated else None
        if self.basename == 'development':
            self.wip = MAVWip()
        else:
            self.wip = soup.findChild('wip', recursive=False)
            self.wip = MAVWip(self.wip) if self.wip else None
        # self.autovalue = autovalue  # True if value was *not* specified in XML
        self.param_fieldnames = set()
        self.params = []
        params = soup.find_all('param')
        for param in params:
            # TODO: Decide if we want to add entries for non-existing param values
            self.params.append(MAVCommandParam(param, self))

    def getMarkdown(self, currentDialect):
        """Return markdown for a command (entry)"""

        string = f"### {self.name} ({self.value})"

        # Add marker after name if there are additions
        if self.basename is not currentDialect or self.deprecated or self.wip:
            string += " —"

        # From dialect to heading if in dialect
        if self.basename is not currentDialect:
            # With basename (dialect name) test
            string += f" \[from: [{self.basename}](../messages/{self.basename}.md#{self.name})\]"
        if self.deprecated:
            string += " [DEP]"
        elif self.wip:
            string += " [WIP]"
        string += ' {#' + self.name + '}\n\n'

        # If it is common we include everything.
        # But for any other dialect we don't include the reset of the message
        if currentDialect == 'common':
            pass
        elif self.basename is not currentDialect:
            return string


        if self.deprecated:
            string += self.deprecated.getMarkdown() + "\n\n"
        if self.wip:
            string += self.wip.getMarkdown() + "\n\n"

        string += f"{fix_add_implicit_links_items(self.description)}\n\n" if self.description else ""
        tableHeadings = []
        tableHeadings.append('Param (Label)')
        tableHeadings.append('Description')
        valueHeading = False
        unitsHeading = False
        multiplierHeading = False
        if any(field in self.param_fieldnames for field in ('enum', 'minValue', 'maxValue', 'increment')):
            valueHeading = True
            tableHeadings.append('Values')
        if 'units' in self.param_fieldnames:
            unitsHeading = True
            tableHeadings.append('Units')
        if 'multiplier' in self.param_fieldnames:
            multiplierHeading = True
            tableHeadings.append('Multiplier')
        tableRows = []

        for param in self.params:
            row = []
            row.append(
                f"{param.index} ({param.label})" if param.label else str(param.index))
            row.append(param.description if param.description else "")

            if valueHeading:
                valString = " "
                if param.enum:
                    valString = fix_add_implicit_links_items(param.enum)
                elif param.minValue or param.maxValue or param.increment:
                    if param.minValue:
                        valString += f"min: {param.minValue}"
                    if param.maxValue:
                        valString += f" max: {param.maxValue}"
                    if param.increment:
                        valString += f" inc: {param.increment}"
                    valString = valString.strip()
                row.append(valString)

            if unitsHeading:
                unitsString = " "
                if param.units:
                    unitsString = param.units
                row.append(unitsString)

            if multiplierHeading:
                multiplierString = " "
                if param.multiplier:
                    multiplierString = param.multiplier
                row.append(multiplierString)

            tableRows.append(row)

        # print("debugtablerows")
        # print(tableRows)

        string += generateMarkdownTable(tableHeadings, tableRows)
        string += "\n\n"

        return string


def tidyDescription(desc_string, type="markdown"):
    """
    Helper method to remove odd whitepace etc from a description string.
    Different behaviour if the string is to be used in normal markdown or in a table.
    """
    if "\n" not in desc_string:
        desc_string = desc_string.strip()
        # print(f"debug1strp|{desc_string}|")
        return desc_string
    if type == "markdown":
        # print(f"debug2|{desc_string}|")
        desc = desc_string
        desc.strip()
        lines = desc.splitlines()
        first_line = lines[0].strip()
        new_string = first_line + "\n\n"
        for line in lines[1:]:
            new_string += line.strip() + "\n"
        desc_string = new_string.strip()
        # print(f"debug3|{desc_string}|")
        return desc_string
    if type == "table":
        lines = desc_string.strip().splitlines()
        new_string = "<br>".join(line.strip() for line in lines)
        # for line in lines:
        #    new_string += line.strip() + "<br>"

        return new_string.strip()


def fix_add_implicit_links_items(input_text):
    if not type(input_text) is str:
        # Its not something we can handle
        return input_text

    # Makes screaming snake case into anchors (helper method). Special fix for MAV_CMD.
    # I don't remember this regexp but it appears to work
    # print("fix_add_implicit_link was called")
    def make_text_to_link(matchobj):
        # print("make_entry_to_link was called: %s" % matchobj.group(0))
        item_string = matchobj.group(2)
        item_url = item_string
        if item_string == 'MAV_CMD':
            item_url = 'mav_commands'
        returnString = f"{matchobj.group(1)}[{item_string}](#{item_url}){matchobj.group(3)}"
        return returnString

    linked_md = re.sub(
        r'([\`\(\s,]|^)([A-Z]{2,}(?:_[A-Z0-9]+)+)([\`\)\s\.,:]|$)', make_text_to_link, input_text, flags=re.DOTALL)
    return linked_md


def generateMarkdownTable(headings, rows):
    """Generates a markdown table from an array containing headings and array containing array for every row."""
    string = ""
    pattern = " | ".join(headings) + "\n"
    string += pattern
    # Generate column marker pattern
    field_count = len(headings)
    pattern = ("--- | ") * (field_count - 1) + "---\n"
    string += pattern
    for row in rows:
        # single elements only get one space
        pattern = "| ".join(f"{elem} " if elem else "" for elem in row) + "\n"
        # print('debug: ROW:')
        # print(row)
        # print(pattern)
        string += pattern
    return string


class XMLFiles(object):
    def __init__(self, dialect=None, source_dir="."):
        self.xml_dialects = dict()
        self.source_dir = source_dir
        if not dialect:
            raise ValueError(
                "XMLFiles requires XML dialect name or list of dialect names")

        dialectNames = []
        if isinstance(dialect, list):
            dialectNames = dialect
        else:
            dialectNames.append(dialect)

        for dialect in dialectNames:
            xmlFileName = f"{self.source_dir}{dialect}.xml"
            print(f"Importing: {xmlFileName}")
            xmlParser = MAVXML(xmlFileName)
            self.xml_dialects[dialect] = xmlParser

        self.expand_includes()
        self.update_includes()  # TODO - make this optional based on a setting?

        """
        # Build a dialect tree for better rendering of included items
        # Dict at top level so we can get self.dialectTree['ardupilotmega']
        # and get a tree we can iterate to print the structure
        # That could be a dict or an array. Probably dict allows more efficiency
        # Might be better if separate to XMLFiles so accessible from where printed.
        # Or we can fetch it with a getter.
        self.dialectTree = {}
        for dialectName in self.xml_dialects.keys():
            print(dialectName)
            if dialectName not in self.dialectTree.keys():
                includes = self.xml_dialects[dialectName].includes

                self.dialectTree[dialectName]=set()
                for dialect in includes:
                    tempDict = {} # {all: {common {stnadard: {minimal: None}} }, common: {} }
        """

    def generateDocs(self, output_dir="."):
        for xmlfile in self.xml_dialects.values():
            xmlString = xmlfile.getMarkdown()

            # Create outputdir if it does not exist
            if not os.path.exists(output_dir):
                os.makedirs(output_dir)

            # output_file = f"{output_dir}{xmlfile.filename}.md"
            output_file = f"{output_dir}{xmlfile.basename}.md"
            with open(output_file, "w", encoding="utf-8") as f:
                print(f"Generating: {output_file}")
                f.write(xmlString)

    def generateIndexDoc(self, output_dir="."):
        # File for index
        index_file_name = "README.md"

        # Create outputdir if it does not exist
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        index_file_name = f"{output_dir}{index_file_name}"

        #initialise text for index file.
        index_text="""<!-- THIS FILE IS AUTO-GENERATED FROM XML: https://github.com/mavlink/mavlink/blob/master/doc/mavlink_xml_to_markdown.py (Do not update mavlink-devguide) -->
# XML Definition Files & Dialects

MAVLink definitions files can be found in [mavlink/message definitions](https://github.com/mavlink/mavlink/blob/master/message_definitions/).
These can roughly be divided into:

- [Standard definitions](#standard-definitions) - core definitions shared by many flight stacks
- [Test definitions](#test-definitions) - definitions to support testing and validation
- [Dialects](#dialects) - *protocol-* and *vendor-specific* messages, enums and commands

## Standard Definitions

The following XML definition files are considered standard/core (i.e. not dialects):

- [minimal.xml](minimal.md) - the minimum set of entities (messages, enums, MAV_CMD) required to set up a MAVLink network.
- [standard.xml](standard.md) - the standard set of entities that are implemented by almost all flight stacks (at least 2, in a compatible way).
  This `includes` [minimal.xml](minimal.md).
- [common.xml](common.md) - the set of entities that have been implemented in at least one core flight stack.
  This `includes` [standard.xml](minimal.md)

> **Note** We are still working towards moving the truly standard entities from **common.xml** to **standard.xml**
  Currently you should include [common.xml](common.md)

In addition:

- [development.xml](development.md) - XML definitions that are _proposed_ for inclusion in the standard definitions.
   These are work in progress.

## Test Definitions

The following definitions are used for testing and dialect validation:

- [all.xml](all.md) - This includes all other XML files, and is used to verify that there are no ID clashes (and can potentially be used by GCS to communicate with any core dialect).
- [test.xml](test.md) - Test XML definition file.

## Dialects  {#dialects}

MAVLink *dialects* are XML definition files that define *protocol-* and *vendor-specific* messages, enums and commands.

> **Note** Vendor forks of MAVLink may contain XML entities that have not yet been pushed into the main repository (and will not be documented).

Dialects may *include* other MAVLink XML files, which may in turn contain other XML files (up to 5 levels of XML file nesting are allowed - see `MAXIMUM_INCLUDE_FILE_NESTING` in [mavgen.py](https://github.com/ArduPilot/pymavlink/blob/master/generator/mavgen.py#L44)).
A typical pattern is for a dialect to include [common.xml](../messages/common.md) (containing the *MAVLink standard definitions*), extending it with vendor or protocol specific messages.

The dialect definitions are:

"""

        for xmlfile in self.xml_dialects.keys():
            index_text+=f"- [{xmlfile}.xml]({xmlfile}.md)\n"

        #Write the index
        with open(index_file_name, 'w') as content_file:
            print(f"Generating: {index_file_name}")
            content_file.write(index_text)


    def expand_includes(self):
        """Expand includes. Root files already parsed objects in the xml list."""

        def expand_oneiteration():
            '''takes the list of xml files to process and finds includes which have not already been turned into xml documents added to
            xml files to process, turns them into xml documents and adds them to the xml files list.
            Returns false if no more documents were added.
            '''
            includeadded = False
            includes_to_add = set()
            for name in self.xml_dialects.keys():
                for incl in self.xml_dialects[name].includes:
                    # print(incl)
                    if incl not in self.xml_dialects:
                        # new include
                        # print(f"debug: {incl} not in {self.xml_dialects.keys()}")
                        includes_to_add.add(incl)
                        includeadded = True
            for incl in includes_to_add:
                xmlFileName = f"{self.source_dir}{incl}.xml"
                print(f"Importing included file: {xmlFileName}")
                xmlParser = MAVXML(xmlFileName)
                self.xml_dialects[incl] = xmlParser
            return includeadded

        for i in range(MAXIMUM_INCLUDE_FILE_NESTING):
            if not expand_oneiteration():
                break

    def update_includes(self):
        """Update dialects and merge with included files,
        starting with the bottom level (files with no or fewer includes).
        Includes were already found and parsed into xml list in expand_includes().
        """

        # 1: Mark files that don't have includes as "done"
        done = []
        for xmldialect in self.xml_dialects.values():
            if len(xmldialect.includes) == 0:
                done.append(xmldialect.basename)
                # print(f"\nFile with no includes found (ENDPOINT): {xmldialect.basename}"  )
        if len(done) == 0:
            print("\nERROR in includes tree, no base found!")
            exit(1)

        # 2: Update all 'not done' files for which all includes have been done.
        #    Returns True if any updates were made

        def update_oneiteration():
            initial_done_length = len(done)
            for xmldialect in self.xml_dialects.values():
                if xmldialect.basename in done:
                    # print(f"Debug: {xmldialect.basename}: already done, skip")
                    continue
                # check if all its includes were already done
                all_includes_done = True
                for i in xmldialect.includes:
                    if i not in done:
                        all_includes_done = False
                        # print(f"debug: {i} not done in {xmldialect.basename}")
                        break
                if not all_includes_done:
                    # print(f"{xmldialect.basename}: not all includes ready, skip")
                    continue

                # Found file where all includes are done
                done.append(xmldialect.basename)
                # print(f"{xmldialect.basename}: all includes ready, add" )
                # now update it with the facts from all it's includes
                for i in xmldialect.includes:
                    # TODO - merge my includes
                    # get the corresponding XML file:
                    dialectToMerge = self.xml_dialects[i]
                    # print(f"debug: merging {dialectToMerge.basename} ({i}) into {xmldialect.basename}")
                    # check that it matches
                    # update the merge
                    xmldialect.mergeIn(dialectToMerge)

                    """
                    fname = os.path.abspath(os.path.join(os.path.dirname(x.filename), i))
                    #print("  include file %s" % i )
                    #Find the corresponding x
                    for ix in xml:
                        if ix.filename != fname:
                            continue
                        #print("    add %s" % ix.filename )
                        x.message_crcs.update(ix.message_crcs)
                        x.message_lengths.update(ix.message_lengths)
                        x.message_min_lengths.update(ix.message_min_lengths)
                        x.message_flags.update(ix.message_flags)
                        x.message_target_system_ofs.update(ix.message_target_system_ofs)
                        x.message_target_component_ofs.update(ix.message_target_component_ofs)
                        x.message_names.update(ix.message_names)
                        x.largest_payload = max(x.largest_payload, ix.largest_payload)
                        break   # WHY DO THIS?
                        """

            if len(done) == len(self.xml_dialects):
                return False  # finished
            if len(done) == initial_done_length:
                # we've made no progress
                print("ERROR include tree can't be resolved, no base found!")
                exit(1)
            return True

        for i in range(MAXIMUM_INCLUDE_FILE_NESTING):
            # print("\nITERATION "+str(i))
            if not update_oneiteration():
                break


def main():
    parser = argparse.ArgumentParser(
        description="Markdown Generator for MAVLink Docs from XML")

    parser.add_argument("-d", "--source_dir", default="../message_definitions/v1.0/",
                        help="Path to XML definition directory")
    parser.add_argument("-i", "--input_dialect", default=None,
                        help="Name of XML dialect, e.g. 'common' (if not specified, does all dialects)")
    parser.add_argument("-o", "--output", default="./messages/",
                        help="Path to Markdown output directory")
    args = parser.parse_args()
    # print(args.source_dir)
    # print(args.input_dialect)
    # print(args.output)

    files = None
    # xml_dialects = [] #The list of dialects to generate markdown for
    if args.input_dialect:
        files = XMLFiles(dialect=args.input_dialect,
                         source_dir=args.source_dir)
    else:
        all_files = os.listdir(args.source_dir)
        xml_dialects = [file[:-4]
                        for file in all_files if file.endswith('.xml')]
        files = XMLFiles(dialect=xml_dialects, source_dir=args.source_dir)
        # xml_dialects.append(f"{args.source_dir}{args.input_dialect}.xml")
    # print(xml_dialects)

    files.generateDocs(args.output)
    files.generateIndexDoc(args.output)


if __name__ == "__main__":
    main()
