#! /usr/bin/python3

"""
Script to extract services summary from a doc

You will need
 apt-get install python3-bs4 
"""

#import lxml.etree as ET
#import requests
from bs4 import BeautifulSoup
#import re
import os


common_file='../message_definitions/v1.0/common.xml'
common_file = os.path.join(os.path.dirname(__file__), common_file)

with open(common_file, 'r') as file:
    data = file.read()

soup = BeautifulSoup(data, 'xml')


# Check that we have a services definition for every service used (helps catch typos)
service = soup.find_all('service')
#print(service)






# List all items by service.
  #List all messages, enums, mavcmds

messages_with_service=soup.findAll('message', service=True)
messages_no_service=soup.findAll('message', service=False)
print("Messages (service): %s" % len(messages_with_service))
print("Messages (noservice): %s" % len(messages_no_service))
#all_enums=soup.findAll('enum')
enums_with_service=soup.findAll( lambda tag: tag.name=='enum' and not tag['name']=="MAV_CMD" and tag.has_attr('service') )
enums_no_service=soup.findAll( lambda tag: tag.name=='enum' and not tag['name']=="MAV_CMD" and not tag.has_attr('service') )
print("Enums (service): %s" % len(enums_with_service))
print("Enums (noservice): %s" % len(enums_no_service))
commands_enum = soup.find( lambda tag: tag.name=='enum' and tag['name']=="MAV_CMD")
commands_with_service = commands_enum.findAll('entry', service=True)
commands_no_service = commands_enum.findAll('entry', service=False)
print("Commands (service): %s" % len(commands_with_service))
print("Commands (noservice): %s" % len(commands_no_service))

#print messages out
 
#message_service_list= [{"name":i['name'],"service":i['service']} for i in messages_with_service]
#print(message_service_list)
service_data=''
for message in messages_with_service:
    service_data+="%s,message,%s\n" % (message['name'],message['service'])
for enum in enums_with_service:
    service_data+="%s,enum,%s\n" % (enum['name'],enum['service']) 
for command in commands_with_service:
    service_data+="%s,command,%s\n" % (command['name'],command['service']) 
for message in messages_no_service:
    service_data+="%s,message,%s\n" % (message['name'],"")
for enum in enums_no_service:
    service_data+="%s,enum,%s\n" % (enum['name'],"") 
for command in commands_no_service:
    service_data+="%s,command,%s\n" % (command['name'],"") 

service_file = os.path.join(os.path.dirname(__file__), 'service.csv')
with open(service_file, 'w') as file:
    file.write(service_data)

print(service_data)

# Get services that are defined
defined_service_names = set([i['name'] for i in service])
print('Service definitions: %s' % defined_service_names)

# Check service defintions have some entities defined.
for service_name in defined_service_names:
    num_items_in_service = len(soup.find_all(service=service_name))
    if not num_items_in_service: print('Warning: "%s" service has no elements.' % service_name)


# List all entities tagged with each service.
# First find what the service tags used are. Print any that have no definition.
#all_tags_with_service_attributes = soup.findAll( lambda tag:tag.has_attr('service') )
all_tags_with_service_attributes = soup.findAll( service=True )
all_tags_with_service_attributes = set([i['service'] for i in all_tags_with_service_attributes])
# print("Services attributes used in elements: %s" % all_tags_with_service_attributes)
for a_service in all_tags_with_service_attributes:
    if a_service not in defined_service_names:
        print('Warning: service "%s" used in elements but not defined (possible typo)' % a_service)


# Other tests?
print('Warning: NO_TEST - Invalid use of service attribute in param, value, whatever. ')
print('Warning: NO_TEST - Test service has version? For this just do 1')




print("COMPLETED")



