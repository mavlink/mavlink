# pydatcom
* A python interface for DATCOM.
* Capabilities
 * Parse
 * Export
 * Plot

## Dependencies

* parsing
 * ply (python lex/yacc)
* exporting
 * jinja templates
* plotting
 * matplotlib

## Example Usage
```bash
pydatcom-export ../test/data/Citation.out -t modelica.mo -o Citation.mo
pydatcom-plot ../test/data/Citation.out
```
