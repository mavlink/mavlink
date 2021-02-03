## MAVLink External Dialects ##

The [/external/dialects](https://github.com/mavlink/mavlink/tree/master/external/dialects) folder hosts MAVLink dialects of *external* projects; projects that are not maintained by core MAVLink stakeholders or part of the MAVLink standard.

> **Note** This mechanism is provided to help non-stakeholder dialect owners avoid clashes with other dialects (and the standard), and to ease integration of generic behaviours into the standard in future.
> We recommend that you work with the standard rather than using this approach if at all possible (there are significant benefits in terms of compatibility and adoptability when using the standard definitions).

Dialects XML files in this folder must meet the following criteria:

* Comply with the [MAVLink XML file format](https://mavlink.io/en/guide/xml_schema.html) (confirm this by using mavgen to build the dialect with validation enabled).
* Metadata, in the form of separate comments near the top of the file, containing:
  * Reliable contact information (email address strongly preferred).
  * Ranges of the message IDs and command IDs that are reserved for the dialect.
    > The range of message IDs and command IDs can be requested in the initial PR.
      Message ids ranges should be above 60000, and are typically reserved in blocks of 50.
  * (Optionally) Link to additional documentation (e.g. a web page)
* Include the dialect in [/mavlink/message_definitions/v1.0/all.xml](https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/all.xml) along with a comment indicating the message/command ranges it uses (the "all" dialect is constantly tested and ensures that messsage IDs and command IDs do not conflict).

You will generally be invited to attend the [Dev Call](https://mavlink.io/en/about/support.html#dev_call) (once) to gain final approval.
This is primarily for the MAVLink team to better understand your needs, and where the core standard can be improved.

MAVlink reserves the right to refuse or remove a dialect that does not comply with the above conditions, or if required due for smooth running of the project (or some other common sense reason). 

Please inspect the dialects in this folder for guidance.


Links:
* [MAVLink XML File Schema / Format](https://mavlink.io/en/guide/xml_schema.html)
* [Dev Call](https://mavlink.io/en/about/support.html#dev_call)

