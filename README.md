Subjective-C: Misadventures In Syntax
=====================================

A library that leverages and showcases either the very best – or the most blatantly horrific – attributes of the OS X Objective-C language and environment alongside those of the LLVM-furnished C++14* language standard and runtime implementation.

<blockquote class="twitter-tweet" data-lang="en"><p lang="en" dir="ltr">Dogg I’m the greatest homeless Objective-C++ programmer on earth; I’m straight up mentally ill with acronym opinions <a href="https://t.co/N4atX8d5Jr">https://t.co/N4atX8d5Jr</a></p>&mdash; Alexander Böhn (@fish2000) <a href="https://twitter.com/fish2000/status/690257722747568128">January 21, 2016</a></blockquote> <script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>

Most of you are probably already too grossed out to keep reading, but for the intrepid nerds who made it past that sentence, a bit of history: I started what would become Subjective-C as a subproject of libimread while I was homeless in Baltimore last summer. Specifically, I was messing around with the Objective-C runtime API and I got to the point where I simply could not stand to look at `objc_msgSend()` any further. 

Not because it’s a problematic idiom or badly implemented runtime call – far from it – but I found the use of `snake_case` and `camelCase`, rammed together in such a short function name, to be such an ignorantly trancendant typographic horror that I could concentrate on little else. This was very useful at that point, historically, since I was homeless in a new city with no friends or money or food, and I was (for a few weeks there) surrounded more or less constantly by junkies, many of whom turned out to be pretty decent theiving robbers despite being so fucked up all of the time – right yes, so TL;DR by focusing on this pressing `objc_msgSend()` “problem” I could marshal my attention well enough to not think about any of all those other things (it was helpful that the junkies did not get my computer and that I found a decent shelter near a McDonalds with WiFi).

Now so listen: I love Objective-C. I love the fact that the bizarre bracket-y message-passing syntax is somehow a legal and strict C-standard superset, I love the logorrhean named-parameter method names that unspool across the screen with all the restraint of an Infinite Jest footnote, I love that dogg fuck namespaces, just use your initials – it’s awesome. It is truly object-oriented and dynamic *and yet* strictly fucking typed, a cocktail of difficulty which makes your working code essentially cleverly super-satisfying *and* provides one the opportunity to witness the public flabbergasting of longtime Java or .NET people when they overconfidently try to Hello World in Xcode on a Saturday afternoon in an Inner Harbor Starbucks.† 

But then also: I really, really like C++. I did not expect that – I only started learned it in 2014; before 2011, C++ was way more busted and uncool, to be sure, yeah. But still. I really really like namespacing things; I really do quite enjoy its lambdas and iterators, whose slight imperfections make them beautiful in my eyes; I seriously fucking like template type inference and SFINAE and the turn-of-phrase a well-thought-out overload resolution heirarchy can provide. Dogg what the fuck, though. Really, these are my feelings.

So anyway, TL;DR – Subjective-C is my paean to these two languages and the clusterfuck that always seems
to result from their combination. Most people prefer their respective strong flavors uncombined – like (say) Talisker and sea-urchin sashimi. Or carpaccio and Fudgie the Whale, maybe. So yeah, if you are into my molecular-gastronomy coding, you are possibly going to love the following:

* Type-safe `objc::msg::send(…)` – a variadic wrapper template around `objc_msgSend()`
    * Automatically substitutes `fpret`/`stret` as appropriate
    * Double-colon looks better than mixed-camel-snake-case

* SFINAE-test for NSObject and friends with `objc::traits::is_object<T>`

* Easy type-safe `NSPasteboard` access:
    
        @autoreleasepool {
            
            std::vector<NSPasteboard*> boards;
            std::unordered_map<path, objc::object<NSImage>> images;
                
            std::for_each(pngs.begin(), pngs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                NSPasteboard* board = [NSPasteboard pasteboardWithUniqueName];
                boards.push_back(board);
            
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                images.insert({ imagepath, objc::object<NSImage>(image) });
            
                BOOL copied = objc::appkit::copy_to(board, image, url);
                CHECK(objc::to_bool(copied));
            });
        
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                BOOL can_paste_url = objc::appkit::can_paste<NSURL>(board);
                BOOL can_paste_image = objc::appkit::can_paste<NSImage>(board);
                CHECK(objc::to_bool(can_paste_url));
                CHECK(objc::to_bool(can_paste_image));
            
                NSURL* url = objc::appkit::paste<NSURL>(board);
                NSImage* boardimage = objc::appkit::paste<NSImage>(board);
                NSImage* mapimage = images.at([url filesystemPath]);
                NSData* boarddata = [boardimage TIFFRepresentation];
                NSData* mapdata = [mapimage TIFFRepresentation];
            
                CHECK(objc::to_bool([boarddata isEqualToData:mapdata]));
            });
        
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                [board releaseGlobally];
            });
        }
    
* AND MORE!!!!

… I am going to document the shit out of all of it, I swear. Right now it doesn't even all compile – I just split it all off from [libimread](http://github.com/fish2000/libimread) but whatevs, you see where I am going with all of this. Yes!


<hr>

† While you swizzle metaclasses and then brag on Yik Yak about what a cool frood you are … seriously tho, they can’t get over the fact that there is no LINQ or DI frameworks. DI stands for “dependency injection” which personally that just makes me very literally think of junkies again, I really do not get why people call it that. Bleah.
