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
common_updated = os.path.join(os.path.dirname(__file__), 'common_updated.xml')
stats_input_file=common_updated

with open(stats_input_file, 'r') as file:
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
  service_description=''
  service_url=''
  try:
    service_description=service_item.description.string
  except:
    pass
  try:
    service_url=service_item.url.string
  except:
    pass
  
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
    version_url=''
    try:
      version_url=version_item['url']
    except:
      pass
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
  if not type or not name or not service or not version:
      print("addelementversion wrong: %s, %s, %s, %s " % (type, name, service, version))
      exit()

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
      print("BIG problem type mismatch: (dict)%s (newtype)%s (elementdict)%s" % (elementdict['type'],type,elementdict) )
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
   print('service_name: %s' % service_name)
   print('service_items: %s' % service_items)
   theversiondict=service_items['versions']
   for version_id, version_items in theversiondict.items():
     print('version_id: %s' % version_id)
     print('version_items: %s' % version_items)
     for amessage in version_items['messages']:
       print('message: %s' % amessage)
       addelementversion('message',amessage,service_name, version_id)
     for acommand in version_items['commands']:
       print('command: %s' % acommand)
       addelementversion('command',acommand,service_name, version_id)
     for aenum in version_items['enums']:
       print('enum: %s' % aenum)
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
    service_string+=service_name
    if len(versions_set)>1:
      #print(len(versions_set))
      service_string+="_v"
      #print(service_name)
      #print(versions_set)
      for version in sorted(list(versions_set)):
        service_string+="_"+version
        #print(version)
    service_string+=','

  service_string+='\n'
  print(service_string)
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



service_file = os.path.join(os.path.dirname(__file__), 'service.csv')
with open(service_file, 'w') as file:
    file.write(service_data)



print("COMPLETED")



