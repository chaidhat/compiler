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
| | | | | | | type: DEREF
| | | | | | | exprsn:
| | | | | | | {
| | | | | | | | {
| | | | | | | | | type: BINARY
| | | | | | | | | stub: 1
| | | | | | | | | single: 0
| | | | | | | | | op (id): +
| | | | | | | | | 
| | | | | | | | | left:
| | | | | | | | | {
| | | | | | | | | | {
| | | | | | | | | | | type: ID
| | | | | | | | | | | isPtr: 0
| | | | | | | | | | | varName: a
| | | | | | | | | | },
| | | | | | | | | },
| | | | | | | | | 
| | | | | | | | | right:
| | | | | | | | | {
| | | | | | | | | | {
| | | | | | | | | | | type: LIT
| | | | | | | | | | | type: INT
| | | | | | | | | | | val: 2
| | | | | | | | | | },
| | | | | | | | | },
| | | | | | | | },
| | | | | | | },
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
