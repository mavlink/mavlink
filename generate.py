#!/usr/bin/env python
"""\
generator.py is a GUI front-end for mavgen, a python based MAVLink
header generation tool.

Notes:
-----
* 2012-7-16 -- dagoodman
    Working on Mac 10.6.8 darwin, with Python 2.7.1

* 2012-7-17 -- dagoodman
    Only GUI code working on Mac 10.6.8 darwin, with Python 3.2.3
    Working on Windows 7 SP1, with Python 2.7.3 and 3.2.3
    Mavgen doesn't work with Python 3.x yet

Copyright 2012 David Goodman (dagoodman@soe.ucsc.edu)
Released under GNU GPL version 3 or later

"""
import os
import re
import pprint

# Python 2.x and 3.x compatability
try:
    from tkinter import *
    import tkinter.filedialog
    import tkinter.messagebox
except ImportError as ex:
    # Must be using Python 2.x, import and rename
    from Tkinter import *
    import tkFileDialog
    import tkMessageBox

    tkinter.filedialog = tkFileDialog
    del tkFileDialog
    tkinter.messagebox = tkMessageBox
    del tkMessageBox

sys.path.append(os.path.join('pymavlink','generator'))
from mavgen import *

DEBUG = False
title = "MAVLink Generator"


class Application(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack_propagate(0)
        self.grid( sticky=N+S+E+W)
        self.createWidgets()
        self.pp = pprint.PrettyPrinter(indent=4)

    """\
    Creates the gui and all of its content.
    """
    def createWidgets(self):


        #----------------------------------------
        # Create the XML entry

        self.xml_value = StringVar()
        self.xml_label = Label( self, text="XML" )
        self.xml_label.grid(row=0, column = 0)
        self.xml_entry = Entry( self, width = 26, textvariable=self.xml_value )
        self.xml_entry.grid(row=0, column = 1)
        self.xml_button = Button (self, text="Browse", command=self.browseXMLFile)
        self.xml_button.grid(row=0, column = 2)

        #----------------------------------------
        # Create the Out entry

        self.out_value = StringVar()
        self.out_label = Label( self, text="Out" )
        self.out_label.grid(row=1,column = 0)
        self.out_entry = Entry( self, width = 26, textvariable=self.out_value )
        self.out_entry.grid(row=1, column = 1)
        self.out_button = Button (self, text="Browse", command=self.browseOutDirectory)
        self.out_button.grid(row=1, column = 2)

        #----------------------------------------
        # Create the Lang box

        self.language_value = StringVar()
        self.language_choices = [ "C", "Python" ]
        self.language_label = Label( self, text="Lang" )
        self.language_label.grid(row=2, column=0)
        self.language_menu = OptionMenu(self,self.language_value,*self.language_choices)
        self.language_value.set(self.language_choices[0])
        self.language_menu.config(width=10)
        self.language_menu.grid(row=2, column=1,sticky=W)

        #----------------------------------------
        # Create the Protocol box

        self.protocol_value = StringVar()
        self.protocol_choices = [ "v0.9", "v1.0" ]
        self.protocol_label = Label( self, text="Protocol")
        self.protocol_label.grid(row=3, column=0)
        self.protocol_menu = OptionMenu(self,self.protocol_value,*self.protocol_choices)
        self.protocol_value.set(self.protocol_choices[1])
        self.protocol_menu.config(width=10)
        self.protocol_menu.grid(row=3, column=1,sticky=W)

        #----------------------------------------
        # Create the generate button

        self.generate_button = Button ( self, text="Generate", command=self.generateHeaders)
        self.generate_button.grid(row=4,column=1)

    """\
    Open a file selection window to choose the XML message definition.
    """
    def browseXMLFile(self):
        # TODO Allow specification of multipe XML definitions
        xml_file = tkinter.filedialog.askopenfilename(parent=self, title='Choose a definition file')
        if DEBUG:
            print("XML: " + xml_file)
        if xml_file != None:
            self.xml_value.set(xml_file)

    """\
    Open a directory selection window to choose an output directory for
    headers.
    """
    def browseOutDirectory(self):
        mavlinkFolder = os.path.dirname(os.path.realpath(__file__))
        out_dir = tkinter.filedialog.askdirectory(parent=self,initialdir=mavlinkFolder,title='Please select an output directory')
        if DEBUG:
            print("Output: " + out_dir)
        if out_dir != None:
            self.out_value.set(out_dir)

    """\
    Generates the header files and place them in the output directory.
    """
    def generateHeaders(self):
        # Verify settings
        rex = re.compile(".*\\.xml$", re.IGNORECASE)
        if not self.xml_value.get():
            tkinter.messagebox.showerror('Error Generating Headers','An XML message defintion file must be specified.')
            return

        if not self.out_value.get():
            tkinter.messagebox.showerror('Error Generating Headers', 'An output directory must be specified.')
            return


        if os.path.isdir(self.out_value.get()):
            if not tkinter.messagebox.askokcancel('Overwrite Headers?','The output directory \'{0}\' already exists. Headers may be overwritten if they already exist.'.format(self.out_value.get())):
                return

        # Verify XML file with schema (or do this in mavgen)
        # TODO write XML schema (XDS)

        # Generate headers
        opts = MavgenOptions(self.language_value.get(), self.protocol_value.get()[1:], self.out_value.get());
        args = [self.xml_value.get()]
        if DEBUG:
            print("Generating headers")
            self.pp.pprint(opts)
            self.pp.pprint(args)
        try:
            mavgen(opts,args)
            tkinter.messagebox.showinfo('Successfully Generated Headers', 'Headers generated succesfully.')

        except Exception as ex:
            if DEBUG:
                print('An occurred while generating headers:\n\t{0!s}'.format(ex))
            tkinter.messagebox.showerror('Error Generating Headers','An error occurred in mavgen: {0!s}'.format(ex))
            return


# End of Application class
# ---------------------------------

"""\
This class mimicks an ArgumentParser Namespace since mavgen only
accepts an object for its opts argument.
"""
class MavgenOptions:
    def __init__(self,language,protocol,output):
        self.language = language
        self.wire_protocol = protocol
        self.output = output
# End of MavgenOptions class
# ---------------------------------


# ---------------------------------
# Start

if __name__ == '__main__':
  app = Application()
  app.master.title(title)
  app.mainloop()
