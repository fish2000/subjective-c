Subjective-C: Misadventures In Syntax
=====================================

A library that leverages and showcases either the very best – or the most blatantly horrific – attributes of the OS X Objective-C language and environment alongside those of the LLVM-furnished C++14* language standard and runtime implementation.

Most of you are probably already too grossed out to keep reading, but for the intrepid nerds who made it past that sentence, a bit of history: I started what would become Subjective-C as a subproject of libimread while I was homeless in Baltimore last summer. Specifically, I was messing around with the Objective-C runtime API and I got to the point where I simply could not stand to look at `objc_msgSend()` any further. 

Not because it’s a problematic idiom or badly implemented runtime call – far from it – but I found the use of `snake_case` and `camelCase`, rammed together in such a short function name, to be such an ignorantly trancendant typographic horror that I could concentrate on little else. This was very useful at that point, historically, since I was homeless in a new city with no friends or money or food, and I was (for a few weeks there) surrounded more or less constantly by junkies, many of whom turned out to be pretty decent theiving robbers despite being so fucked up all of the time – right yes, so TL;DR by focusing on this pressing `objc_msgSend()` “problem” I could marshal my attention well enough to not think about any of all those other things (it was helpful that the junkies did not get my computer and that I found a decent shelter near a McDonalds with WiFi).

Now so listen: I love Objective-C. I love the fact that the bizarre bracket-y message-passing syntax is somehow a legal and strict C-standard superset, I love the logorrhean named-parameter method names that unspool across the screen with all the restraint of an Infinite Jest footnote, I love that dogg fuck namespaces, just use your initials – it’s awesome. It is truly object-oriented and dynamic *and yet* strictly fucking typed, a cocktail of difficulty which makes your working code essentially cleverly super-satisfying *and* provides one the opportunity to witness the public flabbergasting of longtime Java or .NET people when they overconfidently try to Hello World in Xcode on a Saturday afternoon in an Inner Harbor Starbucks.* 

I really, really like C++

* Type-safe `objc_msgSend()`



* While you swizzle metaclasses and then brag on Yik Yak about what a cool frood you are … seriously tho, they can’t get over the fact that there is no LINQ or DI frameworks. DI stands for “dependency injection” which personally that just makes me very literally think of junkies again, I really do not get