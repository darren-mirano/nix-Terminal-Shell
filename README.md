# get_user_input()
Man page for getline() really helped. This let me test code based off of 
their examples and showed the correct arguments to pass. With a bit of 
testing I was able to read in user input. However I did run into a problem
where it was returning the \n. I asked on the 421 discord and a TA told 
me to review the return values on the man page. This helped, and I ended up
just replaceing the newline character with the null terminator. 

# parse_command()
For me, this was the trickiest part of the project. At first I used the 
function strtok_r to split at spaces. This worked until I realized that 
it does not correctly handle quotations. I then scrapped the whole thing
and recoded it with first_unquoted_space(). 

When using first_unquoted_space my first step was to count the number of
arguments the user command had. This allowed me to calloc the right size
for a char** array, this also let me use a for loop without indexing out
of bounds. Then I would simply iterate through the string and strncpy the
substring into a variable and place into the array. Making sure to add
the null terminator at the end of the array and each substring. 

A very challenging function to code, but a very "rewarding" feeling when
it actually worked.
 
# execute_command()
This function didn't take me that long to code. The man pages for wait() and
execvp() really helped. I'm extremely glad we were told to use execvp() since
the exec family has a ton of functions that we could've used. Also learned
about child proccesses and how you need to properly kill the children.
For example, I ran into an issue where if an execvp() failed, the child 
would still be running. So I had to code for that particular issue.

# exit()
For me this wasn't a function, it was just coded in user_promt_loop(). 
Overall pretty simple part to code. I was pretty suprised when I coded it
and it worked on the first time. Just simply get the number of args
and if there is 1 you know the user just entered "exit". If there was 2
you would then have to atoi() on the 2nd arg.

I did however run into a problem where the 2nd argument was letters and
numbers. For example "exit 123trher". This would cause problems as atoi()
would just return that second argument as 0. So the workaround I did was
loop through the string and used isdigit() on each character in the string.
If they were all digits, then atoi(), if they weren't all digits, just 
repromt the user. 

#user_promt_loop()
Last function I coded. Just made a loop and called the functions that 
I coded when neccessary. Also made sure to free each variable as each 
command finished running. 

# proc
For me, proc was a little hard to understand at first. But when I read the 
man page it was clear that it was just a "filesystem". Concatinating the 
two strings was tricky, but researching about strncat() really helped me
put the pieces in the correct spots. My main issue with strncat() was that
I didn't have enough space to hold both /proc and the name of the file. 
Since /proc had to be first. I just initialized the size of that string
to be longer than any of the file names.

Then with the newly concatinated string, you would check if that file 
exists(file* would point to NULL if it didn't) and getline() and printf()
til the end of the file. Making sure to close the file after done reading.

# memory
I made sure that my functions did not leak memory as I coded them. This was
very helpful for me since I didn't have to search throughout all my code if
I was leaking memory. My biggest memory problem was with the char** array.
At first I though free(arr) was good enough. But then I realized you have
to loop through the array and free each individual string. So I made a 
helper function to do that. 
