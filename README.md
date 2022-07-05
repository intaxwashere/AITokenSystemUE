# An AI Token System Plugin for Unreal Engine 5

## What are "tokens"

Not an NFT thing obviously. Tokens (also sometimes called as "tickets" around the industry) are concepts that are used to "allow" specific behaviors in AI agents in games. Each AI (which are "Token Subscriber Component" in our implementation) tries to obtain a specific token type from target (which are "Token Source Component" in our implementation) and can not proceed to next behavior without obtaining it. See [this DOOM GDC](https://youtu.be/2KQNpQD8Ayo) and [this Spider Man GDC](https://youtu.be/LxWq65CZBU8). It's very easy to implement to any tool or system and can be used literally anywhere.

**Real world example:**
- Let's say you have three snipers aiming towards your player, and if all three of them shoot at the same time, your player will die. This is the situation where you use tokens to *decide* which sniper will be able to actually accurately shoot player and others will miss. So your player won't die instantly while it's on it's maximum health value.
- Your game designer is an idiot and created a system where game spawns 50 same type of ranged enemies. If all of them attacks towards single player, your game will both be bottlenecked by the projectiles they spawned and your player won't be able to dodge them and get stressed out. This situation is where you *decide* which enemies will be able to shoot towards players. And meanwhile you will do other things with your AIs that are not managed to obtain tokens so it's still going to look natural. See DOOM's GDC that I posted above to see their real-world example.
- You're doing a horror game like *Slender Man* and you literally only have one single AI in whole gameplay. But you don't want your horror game AI to be very difficulty and creepy at early game, so you add a "Requires Token" condition for some behaviors in Behavior Tree (probably a decorator) and provide those tokens after you reach a certain checkpoint in gameplay. So your AI will evolve over the time with it's *unlocked* behaviors. Alien Isolation also did something similar to this, but they did not use tokens. 

## How to use?

### TL;DR:

- Subclass `UAITokenBase` and define how best subscriber should be selected from given array.
- Create a `UTokenSubscriberComponent` in your "subscriber" (your ai etc)
- Create a `UTokenSourceComponent` in your "source", populate your token categories from details panel in BP. (your player etc)
- Access to your "source" and register your "subscriber" to it with desired token category.
- Port this system to *your favorite AI tool* (be it Behavior Trees, or your custom State Machine implementation or literally just to your game code in Blueprints). Check if your "subscriber" has the given type of token and if it returns true, process your behavior. 
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

## TODOs:

- **Token Stealing**: An implementation that allows other subscribers to steal tokens from their current obtainers.
- **Conditional Token Releasing**: A way to let tokens release theirselves after a specific condition, say it's after a time or a specific event happened in gameplay. But this is also risky because this feature might make the developers' implementations more complex than it has to be, since token obtaining/releasing happens at *your favorite AI tool*. Behavior Trees can't really deal with the situation where tokens are able to release theirselves because of their *"prioritization"* nature. So it might be better to just create an example "manager" or an "extended version of UTokenSubscriberComponent" where they implement conditional releases. So developers can choose to implement it or not.
- **Event-driven tokens**: As you might already noticed (if you already read the source code along with readme) 3rd example I gave above about real world examples (the Alien Isolation / Slender Man one) require event-driven token obtaining and releasing features. Currently, `UTokenSourceComponent`'s loop their `UTokenCategory`'s and evaluate the available tokens *on tick*. If you have a similar logic to what explained in that example, you need to evaluate a simple bool on tick forever until you reach that specific checkpoint. And this both triggers my OCD and looks ugly.
