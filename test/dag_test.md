test/dag_test.mc AST:
{
| {
| | type: VAR
| | {
| | | type: FUNC
| | | funcType: INT
| | | isPtr: 0
| | | isStatic: 0
| | | funcName: a
| | | 
| | | scope:
| | | {
| | | | {
| | | | | type: VAR
| | | | | varName: b
| | | | | varType: INT
| | | | | isPtr: 0
| | | | | isStatic: 0
| | | | | isArray: 0
| | | | },
| | | | {
| | | | | type: ASSIGN
| | | | | varName:
| | | | | {
| | | | | | {
| | | | | | | type: ID
| | | | | | | isPtr: 0
| | | | | | | varName: b
| | | | | | },
| | | | | },
| | | | | 
| | | | | exprsn:
| | | | | {
| | | | | | {
| | | | | | | type: LIT
| | | | | | | type: INT
| | | | | | | val: 2
| | | | | | },
| | | | | },
| | | | },
| | | },
| | },
| },
},
