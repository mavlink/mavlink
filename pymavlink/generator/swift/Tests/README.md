## Javascript MAVLink implementation ##

This code generates ```npm``` modules that can be used with Node.js.  As with the other implementations in Python and C, the MAVLink protocol is specified in XML manifests which can be modified to add custom messages.

*See the gotcha's and todo's section below* for some important caveats.  This implementation should be considered pre-beta: it creates a working MAVLink parser, but there's plenty of rough edges in terms of API.

### Generating the JS implementation ###

Folders in the ```implementations/``` directory are ```npm``` modules, automatically generated from XML manifests that are in the [mavlink/mavlink](https://github.com/mavlink/mavlink) project.  If you wish to generate custom MAVLink packets, you would need to follow the directions there.

You need to have Node.js and npm installed to build.  

To build the Javascript implementations:

```bash
npm install
```