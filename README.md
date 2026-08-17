# RareScript

## What is RareScript?

**RareScript** is a programming language built around a strong type system, configurable type behavior, and a
signal-based event system.

The main idea is that a type in RareScript isn't just a description of some data.

A type can have:

* fields, which represent the data of an object
* properties, which are read-only configuration used by the type itself
* behavior, which defines how the type reacts and operates
* intrinsic behavior, which gives every type a common set of operations
* signals, which allow types and objects to communicate

RareScript is still under development, so this README is not meant to be a complete language specification yet. Some
parts are already defined, while others are still being designed.

---

## Getting Started

A RareScript program is built from **segments**.

```text
[game]
```

A segment determines what kind of code is being described and allows related parts of a program to be grouped together.

More information about the currently available segments:

## `[...]` Segments

### `[game]`

The `[game]` segment is used for game-related code.

```text
[game]
new exp nat(0)
```

Only variables, control-flow blocks, and other **game-logic related code** are allowed inside the `[game]` segment.

Type definitions are **not allowed** in this section. They must be declared in the second segment instead.

This separation is intentional: the `[game]` segment is meant to contain the actual logic that runs as part of the game,
while type definitions belong to a separate section where the structure, properties, and behavior of the program's types
are declared.

For example:

```text
[game]

new player Player(name="John Doe")

if player.hp <= 0 {
    say("${player.name}, you died.")
}
```

Type definitions such as `Player`, however, must be declared in the second segment.

### `[description]`

The `[description]` segment is where you describe the **structure and definitions of your program**.

This is where type-related declarations belong, including:

* type definitions
* type properties
* type behavior
* signals
* other declarations that describe what exists in the program

For example:

```text
[description]

~NewHP {
    hp: nat
}

Player {
    hp: nat
    age: nat
    where
        default = 20,
        min = 1,
        max = 100
}

Player properties {
    immortal: bool
    where
        default = false
}

Player behaviour {
    on NewHP(hp: nat) {
        ...
    }
}
```

The `[description]` segment is essentially where you **define the things that the game can use**, while the `[game]`
segment is where you write the actual game logic.

In a simple way:

```text
[description]
    ↓
"What exists?"

[game]
    ↓
"What does the program do?"
```

This separation keeps type definitions and declarations away from the actual runtime game logic, making the structure of
a RareScript program easier to understand.

---

## Values and Types

Everything in RareScript has a type.

Some built-in types include:

* `nat` -> A type that represent natural number. Even though natural number can reach +infinity, `nat` maximum value is
  `18_446_744_073_709_551_615` (an unsigned 64 bits integer) due to hardware limitation.
* `str` -> A type that represent text. A text here can contain many of things, such as:
    - "Hello World!"
    - "What is your name?"
    - Or even a text inside a text: "My name is : \"John\"!" (you need to escape the inner quotes with `\` to
      disambiguate outer quotes with the inner one)
* `int` -> A type that represent integer number. Even though integer number can reach -infinity to +infinity, `int`
  minimum and maximum values are: `-9,223,372,036,854,775,808` to `9,223,372,036,854,775,807` (signed 64 bits integer)
  due to hardware limitation.
* `bool` -> A type that represent a truthful state, either `true` or `false`.
* `float` -> A type that represent floating-point numbers. It can be used to represent numbers with a fractional part,
  such as `3.14`, `0.5`, or `-12.75`. RareScript uses a `64-bit IEEE 754 floating-point representation` (float64), which
  provides approximately 15–17 decimal digits of precision and a maximum finite value of about `1.7976931348623157 ×
  10^308`. Like other floating-point types, float can introduce small precision errors when representing certain decimal
  values.

---

## Variables

A variable stores a value. In RareScript, variables are explicitly declared with their name and type.

### Declaring a variable

The basic variable declaration syntax is:

```text
new age nat(19)
```

This creates a variable named `age` with the type `nat` and initializes it with the value `19`.

### Changing a variable

Variables can be modified using `set`, `inc`, and `dec`:

```text
set age 20
inc age
dec age
inc age 3
dec age 2
```

`set` replaces the current value:

```text
set age 20
```

`inc` increases the value:

```text
inc age
inc age 3
```

`dec` decreases the value:

```text
dec age
dec age 2
```

When no amount is specified, `inc` and `dec` change the value by `1`.

> Note: `inc` and `dec` only works with numbers.

---

## Defining Types

User-defined types are the main way to create custom data structures in RareScript. A type is declared by giving it a
name followed by a block containing its fields.

```text
Player {
    age: nat
}
```

In this example, `Player` is a new type with one field called `age`. The field has the type `nat`, so every `Player`
object contains an `age` value that follows the rules of `nat`.

A type can contain multiple fields:

```text
Player {
    age: nat
    name: str
    alive: bool
}
```

Here, a `Player` contains three fields:

* `age` — a natural number.
* `name` — a string.
* `alive` — a boolean value.

Fields represent the **state stored by an object** of that type. Each object has its own values for its fields, which
means two objects of the same type can have completely different states.

For example, two `Player` objects could conceptually contain:

```text
player1:
    age = 19
    name = "John"
    alive = true

player2:
    age = 25
    name = "Alice"
    alive = false
```

Both objects have the same structure because they are both `Player`, but their field values are independent from each
other.

### Type definitions are declarations

A type definition doesn't create an object by itself. It describes what a `Player` is and what data an object of that
type should contain.

Objects can then be created from the type:

```text
new player Player(...)
```

The exact construction syntax can depend on the fields and the type's behavior.

Types can also have more than just fields. RareScript allows a type to define **properties** and **behavior**
separately:

```text
Player {
    age: nat
}

Player properties {
    immortal: bool
    where
        default = false
}

Player behaviour {
    when called() {
        say("Oh, Hey there.")
    }
}
```

This gives a type three different parts:

```text
Player
├── Fields       → instance state
├── Properties   → read-only type configuration
└── Behaviour    → how the type operates and reacts
```

This separation is important in RareScript. Fields belong to individual objects, while properties belong to the type and
can be used by the type itself to control its behavior.

---

## Type Properties

Types can define their own properties.

```text
Player properties {
    immortal: bool
    where
        default = false
}
```

Properties are **read-only data used by the type itself**.

They are not ordinary mutable fields.

A type can use its properties to change how it behaves.

For example, `Player` can expose:

```text
immortal
```

and its behavior can use that value when deciding what to do.

Properties can also have properties of their own through `where`.

```text
Player properties {
    immortal: bool
    where
        default = false
}
```

This is possible because properties are typed values, just like other parts of the language.

---

## `where`

`where` is used to **configure the properties exposed by a type**.

A field can specify a type, and that type may provide its own set of properties. The `where` block allows you to provide
values for those properties without modifying the type itself.

For example:

```text
Player {
    age: nat
    where
        default = 20,
        min = 1,
        max = 100,
}
```

Here, `age` is a field of type `nat`. The `where` block configures the properties provided by `nat` for this particular
field.

The important detail is that `default`, `min`, and `max` are **not properties declared by `Player`**. They are
properties provided by `nat`.

Conceptually, `nat` exposes something like:

```text
nat property {
    default: nat
    where
        default = min,

    min: nat
    where
        default = 0,

    max: nat
    where
        default = 18_446_744_073_709_551_615,
}
```

When `where` is used on a field, RareScript looks at the field's type and determines which properties that type
provides.

For example:

```text
age: nat
where
    min = 1,
    max = 100,
    default = 20
```

is essentially saying:

> Use `nat`, but configure its `min`, `max`, and `default` properties with these values.

This also means that `where` is **type-directed**. Different types can expose completely different properties, so the
same syntax can be used to configure completely different kinds of behavior.

For example, `nat` might provide:

```text
default
min
max
```

while another type could provide properties such as:

```text
default
min_length
max_length
```

or:

```text
default
readonly
```

The properties available inside a `where` block therefore depend on the type being configured.

### `where` does not create new fields

Properties configured through `where` are not additional mutable fields of the object.

For example:

```text
Player {
    age: nat
    where
        default = 20,
        min = 1,
        max = 100,
}
```

does **not** mean that every `Player` object now contains:

```text
default
min
max
```

as mutable data.

Instead, those values configure how the `nat` type behaves for `age`.

This distinction is important:

```text
Field      → instance data
Property   → read-only type configuration
```

Because properties are used by the type itself, changing them is not the same thing as modifying normal object state.

### Properties can also be configured on user-defined types

The same mechanism isn't limited to built-in types.

A user-defined type can expose its own properties:

```text
Player properties {
    immortal: bool
    where
        default = false
}
```

Those properties can then be configured wherever the type is used.

This makes `where` an important part of RareScript's type system: **the type defines the properties and their meaning,
while the user provides the configuration.**

---

## Object Construction

Objects are created from a type using the `new` keyword.

The basic syntax is:

```text
new variable Type(...)
```

For example:

```text
new player Player("John", 20)
```

This creates a new `Player` object and stores it in the variable `player`.

The arguments can be passed **positionally**, following the order of the fields defined by the type:

```text
Player {
    name: str
    age: nat
}
```

So:

```text
new player Player("John", 20)
```

sets:

```text
name = "John"
age = 20
```

### Named fields

Arguments can also be passed by explicitly specifying the field name:

```text
new player Player(name="John", age=20)
```

Named arguments make construction more explicit and don't require you to remember the order of the fields.

They can also be useful when a type has many fields:

```text
new player Player(
    name="John",
    age=20,
    alive=true
)
```

### Default construction

A type can also be constructed without explicitly providing a value:

```text
new player Player()
```

This uses the type's intrinsic `@default()` behaviour.

Every type has `@default()` intrinsically, but the default implementation throws an exception. A type must therefore
provide its own implementation if it wants to support default construction.

For example, `nat` provides its own `@default()` behaviour, allowing:

```text
nat()
```

to produce its configured default value.

The same concept applies to user-defined types. Whether:

```text
new player Player()
```

is valid depends on whether `Player` provides a successful default behavior.

### Construction and properties

Properties can affect how construction works.

For example:

```text
Player {
    age: nat
    where
        default = 20
}
```

allows the type's default behavior to obtain the configured value when `age` isn't explicitly supplied.

So construction in RareScript isn't necessarily just about copying arguments into fields. The type itself can
participate in deciding how an object should be constructed.

### Construction styles

In general, RareScript supports these forms:

```text
// Positional construction
new player Player("John", 20)

// Named construction
new player Player(name="John", age=20)

// Default construction
new player Player()
```

The exact rules for mixing positional and named arguments, omitted fields, duplicate named arguments, and
construction-time validation are still being finalized.

---

## Intrinsic Behavior

Intrinsic behaviors are special behaviors provided by RareScript's type system. They define common operations that types
can customize for themselves.

Unlike ordinary behavior, intrinsic behaviors are part of the language's underlying type model. A type can override an
intrinsic behavior when it needs to provide different semantics.

### `@init()`

`@init()` is called **after an object has been initialized**.

At this point, the object already exists and its fields have been initialized, so `@init()` can access the object itself
and its current state through `self`.

```text
when @init() {
    ...
}
```

For example, a type could use `@init()` to perform additional setup after construction:

```text
Player behaviour {
    when @init() {
        this.hp = 100
    }
}
```

The important distinction is that `@init()` happens **after initialization**, rather than being responsible for creating
the object in the first place.

### `@default()`

`@default()` is the intrinsic behavior used when a value needs to be created without explicitly providing one.

Every type has `@default()` intrinsically. However, its default implementation throws an exception instead of producing
a value.

A type therefore needs to override `@default()` if it wants to support default construction.

For example, `nat` overrides `@default()` and returns its `default` property. This makes:

```text
nat()
```

valid.

The relevant properties of `nat` are defined as:

```text
nat property {
    default: nat
    where
        default = min,

    min: nat
    where
        default = 0,

    max: nat
    where
        default = 18_446_744_073_709_551_615,
}
```

So the default value of `nat` is determined through its property configuration.

Conceptually:

```text
nat()
    ↓
@default()
    ↓
nat's implementation
    ↓
default property
    ↓
0
```

The user can change that configuration when using `nat`:

```text 
age: nat
where
    default = 20
```

Now the `nat` used by `age` has `20` as its default value.

This means that every type supports the concept of `@default()`, while whether default construction actually succeeds is
determined by the type itself.

### `@say(text)`

`@say(text)` is the intrinsic behavior used to represent a value as text.

```text
@say(text)
```

Its default representation follows the form:

```text
type-name: text
```

For example, a type named `Player` could produce:

```text
Player: Hello World
```

when its `@say(text)` behaviour is invoked.

This gives every type a basic textual representation, while allowing types to override `@say(text)` when they need a
more appropriate representation.

The exact syntax for invoking `@say()` directly and how custom implementations should construct their textual output are
still being defined.


---

## Behaviour

A type can define its own **behavior** separately from its fields and properties.

While fields describe the state of an object and properties configure how the type behaves, the `behavior` block
contains the actual logic that the type can execute.

A behavior block is declared by placing `behavior` after the type definition:

```text
Player behaviour {
    ...
}
```

For example:

```text
Player {
    name: str
}

Player behaviour {
    when @say(text: str) {
        say("${this.name}: ${text}")
    }

    when called() {
        @this.say("Oh, Hey there")
    }
}
```

Here, `Player` has two pieces of behavior.

The first one overrides the intrinsic `@say(text)` behaviour:

```text
when @say(text: str) {
    say("${this.name}: ${text}")
}
```

Instead of using the default representation of `@say()`, `Player` can define how it should produce text itself. The
`this` keyword refers to the current object, so `this.name` accesses the `name` field of the `Player`.

The second one defines a custom behavior named `called`:

```text
when called() {
    @this.say("Oh, Hey there")
}
```

A behavior can call other behaviors through `@this`. In this example, calling `called()` causes the current `Player`
object to invoke its own `@say()` behaviour.

### `this` and `@this`

RareScript distinguishes between accessing the current object's normal members and invoking its intrinsic behavior.

`this` refers to the current object and is used to access its **fields, properties, and user-defined behaviors**.

For example:

```text
Player behaviour {
    when called() {
        this.name
        this.attack()
    }
}
```

Here, `this.name` accesses a member of the current `Player`, while `this.attack()` calls a user-defined behavior.

`@this` is used specifically for calling **intrinsic behaviors** of the current object.

For example:

```text
Player behaviour {
    when called() {
        @this.say("Oh, Hey there")
    }
}
```

Here, `@this.say(...)` invokes the intrinsic `@say()` behaviour on the current object.

This distinction is intentional:

```text
this.foo()
    → calls a user-defined behaviour

@this.foo()
    → calls an intrinsic behaviour
```

For example, overriding an intrinsic behavior can look like:

```text
Player behaviour {
    when @say(text: str) {
        say("${this.name}: ${text}")
    }

    when called() {
        @this.say("Oh, Hey there")
    }
}
```

`called()` is a user-defined behavior, so it is invoked through the normal `this` mechanism. `@say()` is an intrinsic
behavior, so it is invoked through `@this`.

This keeps the boundary between **ordinary type behavior** and **intrinsic language behavior** explicit.

### Overriding intrinsic behavior

Behavior blocks can also override intrinsic behaviors provided by RareScript.

For example, every type has an intrinsic `@say(text)` behaviour. A type can provide its own implementation using:

```text
when @say(text: str) {
    ...
}
```

Likewise, other intrinsic behaviors such as:

```text
@init()
@default()
```

can be customized by the type when appropriate.

This allows the type itself to decide how fundamental operations should work instead of requiring the language to
hardcode the behavior of every type.

### Custom behavior

Types can define behavior that isn't intrinsic as well:

```text
Player behaviour {
    when called() {
        ...
    }

    when attack() {
        ...
    }
}
```

Custom behavior belongs to the type and can operate on the object's state, call other behaviors, use type properties,
and interact with the rest of the program.

### Signals

Behavior is also where a type can respond to signals.

For example:

```text
~NewHP {
    hp: nat
}

Player behaviour {
    on NewHP(hp: nat) {
        ...
    }
}
```

The `on` form declares a signal handler, while `when` is used for ordinary or intrinsic behavior.

This gives RareScript two closely related mechanisms:

```text
when ...
    → behaviour implemented by the type

on ...
    → behaviour triggered by a signal
```

Together, fields, properties, behavior, and signals allow a RareScript type to describe not only **what an object
contains**, but also **what it can do and how it reacts**.

---

## Signals

RareScript has a signal/event system.

A signal is declared using `~`:

```text
~NewHP {
    hp: nat
}
```

`NewHP` carries a field named `hp`, whose type is `nat`.

Signals are handled by behavior:

```text
Player behaviour {
    on NewHP(hp: nat) {
        set self.hp hp
    }
}
```

A type that declares such a handler can receive `NewHP`.

---

## Emitting Signals

Signals can be emitted at different scopes.

### Global

```text
emit NewHP(100)
```

This emits the signal to all types that receive `NewHP`.

### Type scoped

```text
on Player emit NewHP(100)
```

This emits the signal to all objects whose type is `Player`.

### Object scoped

```text
on player emit NewHP(100)
```

This emits the signal to one specific object.

The three forms can be summarized as:

```text
emit Signal(...)
on Type emit Signal(...)
on object emit Signal(...)
```

The exact dispatch, ordering, lifetime, and error semantics of signals are still being defined.

---

## Background

RareScript actually started from a pretty simple idea.

I wanted to make an RPG game again.

Back when I was still a beginner in programming, one of the things that made programming interesting to me was the idea
of **building my own RPG**. I wasn't thinking much about compiler architecture, type theory, or language design back
then. I mostly just wanted to make a game where I could create characters, enemies, items, stats, quests, and all the
other things you usually find in an RPG.

Years later, I still wanted to make that kind of game again.

But this time, I wanted to approach it differently.

Instead of immediately using an existing game engine and its scripting language, I started thinking about what a
programming language designed around RPG and game development would actually look like.

That eventually turned into RareScript.

At first, the idea was pretty ordinary: create a language that could be used to describe game objects and their
behavior. But while designing it, I became more interested in the language itself than just the game.

I started asking questions like:

> What if a type could control how it behaves?

> What if types could expose their own configuration instead of everything being hardcoded into the language?

> What if default values, constraints, and other behaviors could be expressed through the type system?

> What if events and signals were also part of the language instead of something bolted on afterward?

Those ideas gradually became the foundation of RareScript.

So although RareScript is heavily influenced by the goal of making games—especially RPGs—it isn't meant to be just a
"game scripting language."

It's becoming an experiment in **type systems, language design, and how much behavior a type can own**.

The RPG idea is still somewhere at the center of it, though.

I still want to use RareScript to build that RPG eventually. The difference is that now I'm also interested in seeing
**what kind of programming language naturally grows out of trying to build it**.

That's probably why RareScript ended up becoming much more ambitious than the original idea.

It started with:

> "I want to make an RPG again."

And slowly turned into:

> "What if I built the language I wish I had for making it?"

---

## Status

RareScript is still being built.

The type system, parser, compiler, syntax, runtime, and standard library are all evolving. Some things that look strange
today may end up being completely redesigned later.

The README will grow alongside the language.

For now, the easiest way to understand RareScript is to remember the core idea:

> **Types aren't passive descriptions of data. They can define the rules for how that data behaves.**

And that's basically where RareScript starts getting interesting.

