# 5EXorD - what is it?
5EXorD is a lossless string encryption system developed by TheDolomit/Vhyrro.
Originally created as a joke, I realized that this encryption system has some potential, so I decided to share it!
# How safe is it?
EXorD uses a 192bit (3x64bit) encryption key to encrypt your message, and even uses random number generators to ensure that no two encryptions will be the same.
# What about speed?
EXorD is based solely off of primitive operations (BIT AND, BIT OR, XOR) meaning that there are no complex and convoluted mathematical algorithms to be seen.
# How big should I expect my encrypted strings to be?
EXorD generates an encrypted string that is 2x larger (+ some extra characters) than the original input, which is quite impressive for one of my first encryption systems.
# How does it work?
A detailed description of how this works will soon appear on the repo's wiki. Currently, we have some basic information regarding the concepts, but more advanced and practical things will be coming as soon as I have the time to write about them, so stay tuned!
# Ok, great, but what about... Licensing?
The .exe of software alongside the source code is completely free - use it however the hell you want, you can even claim you made it yourself for all I care - commercial, personal use, doesn't matter. The code you see is somewhat more of a proof of concept than anything (doesn't mean you shouldn't use it though haha) meaning if you have a better implementation, please let me know!
# About that exe, what is it?
It's the code you see in the Main.cpp file, it allows you to encrypt/decrypt strings and even files. Useful if you don't want to strictly use the API, but just want to encrypt something.
# Vulnerabilities
If you find *any* sort of vulnerabilities then please let me know as soon as possible! :)
# What can I expect to see in upcoming releases?
Definitely more security updates (once again, if you find any security flaws, let me know!).  
Currently, EXorD has a lot of difficulty with encrypting/decrypting binary files (.wavs, .pngs etc.)  
I have recently started working on fixing these issues, so expect to be able to encrypt any type of file soon!
# I don't know how to use the exe, help :(
Run the .exe and enter the mode that you want to put the application into (E for encryption, D for decryption, FE for file encryption, DE for file decryption). Afterwards, provide the application with three numbers (can be any numbers you want) - hold on to these numbers as you will need them for decryption. Later, when you are ready to decrypt, simply enter the encrypted string, reenter the correct three numbers that you used for encryption aaand viola! You have your decrypted string back.
