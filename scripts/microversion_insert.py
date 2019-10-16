#! /usr/bin/python3

"""
Script to add services in srvice.csv to common.xml

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
service_file = os.path.join(os.path.dirname(__file__), 'service.csv')


services_list=dict()
with open(service_file, 'r') as file:
    line = True
    cnt = 1
    while line:
        line = file.readline().strip()
        linedata=line.split(',')
        if len(linedata)<3:
            continue
        try:
            element_type=linedata[0]
            element_name=linedata[1]
            for i in range(2,len(linedata)):
                #each iteration is a service with some versions
                if len(linedata[i].strip())==0:
                    continue
                print('service(%s): %s' % (i-1,linedata[i]))
                split_versions=linedata[i].split('_v_',1)
                service=split_versions[0].strip()
                print(service)
                if len(split_versions)==1: #just the service
                    versions=["1",]
                else:
                    versions=split_versions[-1].split('_')
                print('vers:%s' % versions)



                if not service in services_list: #new service
                    #print("SL1: %s" % services_list)
                    aservice=dict() # name, version object, with multiple version items
                    aservice['name']=service
                    aservice['versions']=dict() #object to hold y version items
                    for eachversion in versions:
                        aversion=dict() #id, types.
                        aversion['id']=eachversion
                        definition_set=set()
                        definition_set.add(element_name)
                        aversion[element_type]=definition_set
                        aservice['versions'][eachversion]=aversion
                    services_list[service]=aservice
                    #print("SL1: %s" % services_list)
                else: #service is in list.
                    #print('Element: name(%s) type(%s)' % (element_name,element_type))
                    #print('Versions: (%s) ' % (versions))
                    #print("SIL")
                    the_servicedict=services_list[service]
                    the_versions_dict=the_servicedict['versions']
                    #print("the_versions_dict: %s" % the_versions_dict)
                    for eachversion in versions:
                        #print("eachversion: %s" % eachversion)
                        if eachversion in the_versions_dict:
                            aversion=the_versions_dict[eachversion]
                        else:
                            aversion=dict()
                            aversion['id']=eachversion
                        #print("aversion: %s" % aversion)                     
                        if element_type in aversion: 
                            definition_set=aversion[element_type]
                        else:
                            definition_set=set()
                        definition_set.add(element_name)
                        aversion[element_type]=definition_set                


                print('SL: %s' % services_list)
                     
                #print(len(versions)) 
                
        except:
            pass

for service_key, service_value in services_list.items():
    service_text='<service name="%s">\n' % service_key
    for versionkey, version_value in service_value['versions'].items():
        service_text+='  <version id="%s">\n' % versionkey
        #version_text='<version id="%s">\n</version>\n' % versionkey
        #print(version_text)
        #print('version_value: %s' % version_value)
        for deftypekey, deftype_value in version_value.items():
            if deftypekey=='id':
                continue
            #print('DEFVAL: %s' % deftype_value)       
            #print('DEFKey: %s' % deftypekey)
            for an_element in deftype_value:
                service_text += '    <%s>%s<%s>\n' % (deftypekey,an_element,deftypekey)
                #print(element_text)
        service_text+='  </version>\n'
    
    service_text+='</service>\n'

    print(service_text)

# Open common.xml and update it with 
with open(common_updated, 'w') as file:
    file.write(service_text)


"""
<services>
  ...
  <service name=”params” id=”n”>
    <description>Optional description of service</description>
    <url>Optional URL for service docs. Either points to current version or parent doc.</url>
    <version=”1” url=”http://docs_for_version_if_exist”>
     <message>MESSAGE1</message>
    </version>
    <version=”2”>
     <message>MESSAGE1</message>
     <message>MESSAGE2</message>
     <enum>ENUM1</enum>
     <command>MAV_CMD_...</command>
    </version>
  </service>
  ...
</services>
"""

"""
# Open common.xml and update it with 
with open(common_file, 'r') as file:
    data = file.read()


soup = BeautifulSoup(data, 'xml')

#print('Services: %s' % services_list)
#print('Elements: %s' % elements_with_services)

#Check we have a services block
services = soup.find('services')
if not services:
    print("create services block")
    services_tag = soup.new_tag('services')
    #services.string = 'ever'
    soup.dialect.insert_after(services_tag)


# Open common.xml and update it with 
with open(common_updated, 'w') as file:
    file.write(soup.prettify(formatter=None))



print("COMPLETED")

"""
"""
with open(service_file, 'w') as file:
    file.write(service_data)




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


"""





