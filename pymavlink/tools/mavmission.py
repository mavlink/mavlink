#!/usr/bin/env python

'''
extract all MAVLink mission sets from logs
'''

import sys, time, os
from argparse import ArgumentParser

from pymavlink import mavutil, mavwp
import simplekml

class WaypointManager:
    '''Helper class for managing waypoints. Provides a simple API for storing waypoints and outputting them to a file.'''
    formats = ['waypoints', 'kml']

    def __init__(self, type):
        self.type = type
        self.waypoint_count = 0
        self.waypoints = []
        if type == 'waypoints':
            self.manager = mavwp.MAVWPLoader()
        elif type == 'kml':
            self.manager = simplekml.Kml()
            self.waypoint_folder = self.manager.newfolder(name="Missions")
        else:
            raise ValueError('Type must be one of "waypoints", "kml"')

    def clear(self):
        '''Clear the internal waypoint list'''
        self.__init__(self.type)

    def add(self, waypoint):
        '''
        Append a waypoint to the end of the list.

        waypoint is a 3-tuple of latitude/longitude/altitude.
        '''
        self.waypoints.append(waypoint)
        if self.type == 'waypoints':
            self.manager.add(waypoint)
        elif self.type == 'kml':
            self.waypoint_folder.newpoint(coords=[(waypoint.y, waypoint.x, waypoint.z)],
                                  name="Mission {}".format(self.waypoint_count))

        self.waypoint_count += 1

    def count(self):
        '''Getter for the number of waypoints currently in the list.'''
        return self.waypoint_count

    def save(self, filename):
        '''Save the current mission list to a file.'''
        if self.type == 'waypoints':
            self.manager.save(filename)
        elif self.type == 'kml':
            path = self.manager.newlinestring(name="Path")
            path.coords = [(w.y, w.x, w.z) for w in self.waypoints]
            path.altitudemode = simplekml.AltitudeMode.relativetoground
            path.extrude = 1
        self.manager.save(filename)


# Configure and parse command line arguments
parser = ArgumentParser(description=__doc__)
parser.add_argument("--dialect", default="ardupilotmega", help="MAVLink dialect")
parser.add_argument("--no-timestamps", dest="notimestamps", action='store_true', help="Log doesn't have timestamps")
parser.add_argument("logs", metavar="LOG", nargs="+")
parser.add_argument("--format", default='waypoints', choices=WaypointManager.formats, help='Specify output format. Either QGroundControl\'s .waypoints format or KML.')
args = parser.parse_args()

def mavmission(logfile):
    '''extract mavlink mission'''
    mlog = mavutil.mavlink_connection(filename,
                                      dialect=args.dialect,
                                      notimestamps=args.notimestamps)

    # Generate the base name for all mission files
    if args.format == 'waypoints':
        filename_base = os.path.splitext(filename)[0] + "-%d.waypoints"
    else:
        filename_base = os.path.splitext(filename)[0] + "-%d.kml"

    manager = WaypointManager(args.format)
    total_mission_count = 0
    mission_set = 0

    while True:
        # Handle both the MISSION* messages from MAVLink 1.0 and the WAYPOINT* messages from
        # MAVLink 0.9. Additionally the CMD message from the DF log format is supported here,
        # though it doesn't support multiple mission sets.
        m = mlog.recv_match(type=['MISSION_COUNT', 'MISSION_ITEM', 'WAYPOINT_COUNT', 'WAYPOINT', 'CMD'])

        # End the loop once there are no more messages
        if m is None:
            break

        # If this is a DFlog, process the CMD messages as well.
        if m.get_type() == 'CMD':
            # Create a MISSION_ITEM message from this CMD message to simplify the code
            m = mavutil.mavlink.MAVLink_mission_item_message(0,
                0,
                m.CNum,
                mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
                m.CId,
                0, 1,
                m.Prm1, m.Prm2, m.Prm3, m.Prm4,
                m.Lat, m.Lng, m.Alt)

        # Start a new mission file when we receive a MISSION_COUNT message
        if m.get_type() in ('MISSION_COUNT', 'WAYPOINT_COUNT'):
            manager.clear()
            total_mission_count = m.count
            continue

        # Fill in missing waypoints so that at least a partial listing can be extracted
        while m.seq > manager.count():
            print("Warning: Adding dummy WP %u" % manager.count())
            manager.add(m)

        # Add the new mission into the list
        manager.add(m)

        # If this is the last mission, output this mission set
        if m.seq == total_mission_count - 1:
            save_filename = filename_base % mission_set
            manager.save(save_filename)
            mission_set += 1
            print("Saved %u waypoints to '%s'" % (manager.count(), save_filename))

for filename in args.logs:
    mavmission(filename)
