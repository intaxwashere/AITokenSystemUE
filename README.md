# An AI Token System Plugin for Unreal Engine 5

## What are "tokens"

Not an NFT thing obviously. Tokens (also sometimes called as "tickets" around the industry) are concepts that are used to "allow" specific behaviors in AI agents in games. Each AI (which are "Token Subscriber Component" in our implementation) tries to obtain a specific token type from target (which are "Token Source Component" in our implementation) and can not proceed to next behavior without obtaining it. See [this DOOM GDC](https://youtu.be/2KQNpQD8Ayo) and [this Spider Man GDC](https://youtu.be/LxWq65CZBU8). It's very easy to implement to any tool or system and can be used literally anywhere.

## How to use?

### TL;DR:

- Subclass `UAITokenBase` and define how best subscriber should be selected from given array.
- Create a `UTokenSubscriberComponent` in your "subscriber" (your ai etc)
- Create a `UTokenSourceComponent` in your "source", populate your token categories from details panel in BP. (your player etc)
- Access to your "source" and register your "subscriber" to it with desired token category.
- Port this system to your favorite AI tool (be it Behavior Trees, or your custom State Machine implementation or literally just to your game code in Blueprints). Check if your "subscriber" has the given type of token and if it returns true, process your behavior. 
- For BT's using a service to obtaining tokens, and using a decorator to checking if subscriber has the desired token is recommended. For other tools, it's going to be up to you since FSM's and your game code will have more options.

### Details:

Basically, this plugin introduces four classes:

- Tokens
- Token Categories
- Token Subscriber Components
- Token Source Components

### Tokens

Tokens live inside `UTokenCategory`'s and `UTokenSubscriberComponent`'s. They determine how and which subscriber can obtain them. They select best/desired subscriber with `TryGetToken()` function. They get "used" after obtained and "released" after their owner (subscriber component) releases them.

### Token Categories

Token Categories are collection of Tokens. Each category can have it's unique type of token and populate requested amount of them. They only live inside `UTokenSourceComponent`'s and engine will throw an assertion if user attemps to construct them outside of their context.

### Token Subscriber Components

Subscriber Components *register* theirselves to `UTokenSourceComponent`'s. They hold the obtained tokens and only act like a container of tokens. On destroyed they release all obtained tokens.

### Token Source Components

Token Source Components hold a TMap of Token Subscriber Components and Token Categories and they evaluate them on tick. Token Subscribers registers theirselves to this component with their desired Token Category type. 
