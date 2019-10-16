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



services = soup.find('services').find_all('service')
#print(services)

# Create a dict to represent all our services
services_dict=dict()
for service_item in services:
  #print('Service: %s' % service_item)
  service_name=service_item['name']
  service_id=service_item['id']
  service_description=service_item.description.string
  service_url=service_item.url.string
  #print(service_name)
  #print(service_id)
  #print(service_description)
  #print(service_url)
  service_dict=dict() # A dict for each service accessed by name. Contains versions
  service_dict['name']=service_name
  service_dict['id']=service_id
  service_dict['description']=service_description
  service_dict['url']=service_url
  #print(service_dict)
  versions=service_item.find_all('version')
  versions_dict=dict()
  for version_item in versions:
    #print(version_item)
    version_id=version_item['id']
    version_url=version_item['url']
    #print(version_id)
    #print(version_url)
    version_dict=dict()
    version_dict['id']=version_id
    version_dict['url']=version_url
    message_set=set()
    enum_set=set()
    command_set=set()
    messages=version_item.find_all('message')
    for message_item in messages:
        message=message_item.string
        message_set.add(message)
        #print(message)
    enums=version_item.find_all('enum')
    for enum_item in enums:
        anenum=enum_item.string
        enum_set.add(anenum)
        #print(anenum)
    commands=version_item.find_all('command')
    for command_item in commands:
        command=command_item.string
        command_set.add(command)
        #print(command)
    version_dict['messages']=message_set
    version_dict['commands']=command_set
    version_dict['enums']=enum_set
    #print('VERSION: %s' % version_dict)
    versions_dict[version_id]=version_dict
  service_dict['versions']=versions_dict
  services_dict[service_name]=service_dict

#print('SERVICES: %s' % services_dict)


# Below organises data by elements. 
element_with_service=dict()

def addelementversion(type,name,service, version):
  """Adds items to element_with_service"""
  if not name in element_with_service:
    elementdict=dict()
    elementdict['name']=name
    elementdict['type']=type
    elementdict['services']=dict()
    services=dict()
    versions=set()
    versions.add(version)
    services[service]=versions
    elementdict['services']=services
    element_with_service[name]=elementdict
  else:
    elementdict=element_with_service[name]
    if elementdict['type']!=type:
      print("BIG problem type mismatch")
      exit()
    if elementdict['name']!=name:
      print("BIG problem name mismatch")
      exit()
    services=elementdict['services']
    if not service in services: 
      # This is a new service. Need to create new version
      versions=set()
      versions.add(version)
      services[service]=versions
    else: #service exists. Just add a new version 
      versions=services[service]
      versions.add(version)
      services[service]=versions
    elementdict['services']=services
    element_with_service[name]=elementdict


for service_name, service_items in services_dict.items():
   #print('service_name: %s' % service_name)
   #print('service_items: %s' % service_items)
   theversiondict=service_items['versions']
   for version_id, version_items in theversiondict.items():
     #print('version_id: %s' % version_id)
     #print('version_items: %s' % version_items)
     for amessage in version_items['messages']:
       #print('message: %s' % amessage)
       addelementversion('message',amessage,service_name, version_id)
     for acommand in version_items['commands']:
       #print('command: %s' % acommand)
       addelementversion('command',acommand,service_name, version_id)
     for aenum in version_items['enums']:
       #print('enum: %s' % aenum)
       addelementversion('enum',aenum,service_name, version_id)
#print("ELEMENTS BY SERVICE: %s " % element_with_service)
     

# Now print them out like
#element (name) : name, type, services - versions list
#item, type, servicename_v_1_2_x, servicename_v_1_2 


service_data=''
for definition_name, definition_info in element_with_service.items():
  theservices=definition_info['services']
  service_string=definition_info['type']+','+definition_name+','
  
  for service_name, versions_set in theservices.items():
    service_string+=service_name+"_v"
    #print(service_name)
    #print(versions_set)
    for version in sorted(list(versions_set)):
      service_string+="_"+version
      #print(version)
    service_string+=','

  service_string+='\n'
  #print(service_string)
  service_data+=service_string
#print(service_data)

#The add all other data that has no services.

messages_all_soup=soup.find('messages').findAll('message')
for message in messages_all_soup:
  if not message['name'] in element_with_service:
    service_data+="message,%s\n" % message['name']

enums_all_soup=soup.find('enums').findAll('enum')
for myenum in enums_all_soup:
  if not myenum['name'] in element_with_service:
    service_data+="enum,%s\n" % myenum['name']

commands_all_enum =soup.find('enums').find( lambda tag: tag.name=='enum' and tag['name']=="MAV_CMD").findAll('entry')
for acommand in commands_all_enum:
  if not acommand['name'] in element_with_service:
    service_data+="command,%s\n" % acommand['name']

 
print(service_data)

"""
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
"""


service_file = os.path.join(os.path.dirname(__file__), 'service.csv')
with open(service_file, 'w') as file:
    file.write(service_data)


"""

# List all items by service.
  #List all messages, enums, mavcmds



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


"""

print("COMPLETED")



