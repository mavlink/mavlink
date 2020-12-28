## MAVLink External Dialects ##

The mavlink/external/dialects folder hosts MAVLink dialects of external projects, which can include also non-professional projects.

These dialects are not maintained by MAVLink nor are part of the MAVLink standard, but are in the sole responsibility of the dialect owner.

New dialects can be requested to be added by following the below common sense rules and practices, which are designed to help the dialect owner with avoiding clashes with other dialects and ensuring conformity with the MAVLink xml file format.

* Add a comment near the top of the dialect with a reliable contact mechanism, such as an email or whatever approach is prefered.
* Add a comment near the top of the dialect indicating the ranges of the message IDs and command IDs which are reserved for the dialect.
* Consider adding a comment near the top of the dialect, which contains or directs to some documentation, such as a link to a web page or wiki or whatever approach is prefered. (not mandatory)
* Ensure that the dialect meets the MAVLink xml file format.
* Include the dialect in all.xml in the folder mavlink/message_definitions/v1.0. (all.xml is constantly tested and ensures that messsage IDs and command IDs do not conflict)
* The range of message IDs and command IDs can be requested and formal approval be obtained by attending the Dev Call once for that purpose (https://mavlink.io/en/about/support.html#dev_call).

Please inspect the dialects in this folder for guidance. The message and command IDs should be above 60000, and typically are reserved in blocks of 50.

Having to attend the Dev Call once may seem like a serious step, but it is desired to ensure a valid point of contact and a certain level of commitment. Don't be afraid, it's nothing more (and less) than the online version of getting to know each other over a beer.

MAVlink reserves the right to remove a dialect in singular cases if common sense suggests so.

Links:
* [MAVLink XML File Schema / Format](https://mavlink.io/en/guide/xml_schema.html)
* [Dev Call](https://mavlink.io/en/about/support.html#dev_call)

