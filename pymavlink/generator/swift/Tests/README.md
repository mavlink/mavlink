## Swift MAVLink implementation test project ##

MAVLinkTests target covers `DataExtensions` with unit tests (conversion between raw data and typed values). Also it contains some integration tests for `MAVLink` class.

To quickly generate MAVLink classes and enums for Swift you can use following script:

```bash
./ardugen.sh
```
It will generate Swift files for `ardupilotmega.xml` definitions into `MAVLink` folder inside test project.