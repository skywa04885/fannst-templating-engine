![Fannst Banner](static/fannst-banner.png)

# Simple C++ Templating Engine - High performance target

### Syntax Example: 

```html
<div class="example">
    <h1>{%= $title %}</h1>
    <p>{%= $var %}</p>
    <p><small>{%= $bool %}</small></p>
</div>
```

## Syntax Rules

1. Each command starts with ```{%``` and ends with ```%}```
1. The first char inside a command, should represent the type of operation, for supported operations, 
see section "Supported Operations"
1. Each functional operation requires an label, for more information see section "Functional Operations"

## Supported Operations

* ```=``` Insert Text, HTML Tags will be processed into HTML Entities
* ```-``` Insert HTML, no preprocessing performed
* ```:``` Functional operation, will contain if statements and loops

## Functional Operations

### Rules

1. Each functional operation requires an label, this can be specified in this 
manner: ```{%: labelName | if $bool %}```
1. Labels may be created using alpha numerical characters A-Z, a-z
1. After each label, and ```|``` is required, in order to indicate that the statement is comming
1. Due to performance, booleans are the _only_ supported statement operands

### Examples

#### If statement

```
{%: shouldShowParagraph | if $bool %}
    <p>{%= $text %}</p>
{%: shouldShowParagraph | endif %}
```

## Variable Insertions

### Supported variables

* Booleans
* ```const char *```'s
* Double's
* Float's
* Integer's
* Long's

## Developers

|Name          |Github                                         |Website                                  |
|--------------|-----------------------------------------------|-----------------------------------------|
|Luke Rieff    |[skywa04885](https://github.com/skywa04885)    |[me.fannst.nl](https://me.fannst.nl)     |

Want to become an developer for Fannst Software ? 
Feel free to contact us for more information, keep in mind that you will be doing
this as a volunteer, and will not get payed. Click [Here](https://software.fannst.nl) to get in touch.

---