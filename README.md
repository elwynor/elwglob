<pre>
                         ________      __          __
                        / ____/ /___  / /_  ____ _/ /____
                       / / __/ / __ \/ __ \/ __ `/ / ___/
                      / /_/ / / /_/ / /_/ / /_/ / (__  )
                      \____/_/\____/_.___/\__,_/_/____/
                      
                                version 3.43
                    Copyright (c) 2024 Elwynor Technologies
</pre>

## WHAT IS GLOBALS?

 Globals is a comprehensive global commands package that includes a number of
 global commands and capabilities. 

## HOW TO USE GLOBALS
<pre>
 Here are some general tips to help you with Globals. A toggle is a command
 that is like a light switch; you can turn it ON or OFF. When you see the <> or
 [] brackets around some text, that means that you can enter something at the
 point where the brackets are. For example, you can get help on any global
 command by typing =? <global>, and you would replace <global> with the command
 you want help with, for example =cc (for chat channels). When the term "Sysop"
 is used, it means a staff member with access to do something, and not just
 someone in the SYSOP class.

 When you see (+), this means you can optionally type + after the command to
 inform the user of what you've done. For example, you may or may not want
 users to know when you've given or revoked a key, and if you want them to
 know, you would type + at the end of the command. An example of this would be
 if you wanted to give John the adult key, and wanted to let him know you gave
 it to him. You would type:

 =uk John ADULT +

 This would appear in the help as: =uk <user-id> [key] (+)

 Below is a listing of the globals commands with an explanation of how to
 use them.
</pre> 
## GLOBAL COMMAND HELP   
<pre>
 You can get a listing of all global commands you can use and their general
 function by typing =?.
</pre>    
### HELP WITH SPECIFIC COMMANDS
<pre>
 Typing =? <global command> will print a detailed description of that global
 command's function. For non-toggle commands simply entering the command
 string, such as =pc for post credits, will display help on that command's
 usage. Note: Toggle commands are globals such as the ANSI toggle, which you do
 not type anything after to use, and which turn something on and off, or
 "toggle" it.
</pre>    
### INSTANT LOGOFF
<pre>
 Typing =x from anywhere on the system will log you off the BBS immediately.
</pre>    
### PAGE CONSOLE
<pre>
 Typing =e will make a beep at the sysop's computer console. It may also page
 other assistants who are on-line. Multiple beeps may be configured by the
 Sysop, or they may be turned off. Sysops please note that you can find out who
 paged you by going to your Audit Trail detail screen.

 Sysops may end the current page by typing =q. You may wish to do this if you
 have set multiple beeps, and are currently on-line so that the ongoing beeps
 don't drive you nuts. If you are not on-line but are at the main console, you
 can type Alt-Q to end the beeps. You will need to hold the Alt-Q keys down
 until the beeping stops.
</pre>    
### ANSI SETTING TOGGLE
<pre>
 Typing =a will toggle your ANSI setting on and off.
</pre>    
### BUSY FLAG TOGGLE
<pre>
 Typing =b will toggle your "Busy" flag on and off. Leaving the flag on allows
 you to receive pages and other interruptions, while turning it off disables
 them.

 Your "Busy" flag will be left on until you turn it back off again; it is not
 turned back off when you log-off the system.

 Sysops can also turn another user's "Busy" flag OFF. This is done by typing =b
 <userid>. This cannot be done if <userid> is transferring a file or in Opchat.
</pre>    
### BUSY NOTIFY
<pre>
 Typing =bn <user-id> will allow you to be notified when <user-id> is no longer
 "Busy" and can be paged. You can view your current notification at any time by
 typing =bn ?, and you can end your current notification by simply typing =bn.
 You may only have one notification at a time; if you use the command again
 while you are waiting for your current notification, only the last
 notification will be executed. Note that if you are "Busy" when your
 notification comes, you will be notified as soon as you are no longer "Busy".
</pre>    
### DUPLEX MODE TOGGLE
<pre>
 Typing =du will toggle your duplex, or echo, setting. Modes are full duplex,
 which echos your input on the BBS, and half duplex, which does not echo your
 input. What this means is that when your input is echoed, you see what you
 type, and when it is not echoed, you don't see what you type.
</pre>    
### EDITOR TYPE TOGGLE
<pre>
 Typing =ed will toggle your default editor type from line
 to full screen and vice versa.
</pre>    
### RELOGIN
<pre>
 Typing =re will re-log you onto the board; it is the same as the 'R' option at
 the exit menu if you have that option. Sysops may also force another user to
 re-log. This is done by typing =re <user-id> (+). You cannot re-log a user who
 is in Opchat or transferring a file.
</pre>    
### MEMBERSHIP INFO
<pre>
 Typing =$ will display information about board membership and how to join.
</pre>    
### IBM MODE TOGGLE
<pre>
 Typing =g will toggle your system type as being as IBM-PC on and off. This
 affects the graphics characters you can see.
</pre>    
### SCREEN WIDTH SETTING
<pre>
 Typing =sw will set your screen width in characters to between 40 and 136.
</pre>    
### SCREEN LENGTH SETTING
<pre>
 Typing =s will set your screen length in lines to between 3 and 254.
</pre>    
### CONTINUOUS OUTPUT TOGGLE
<pre>
 Typing =ct will toggle your screen output setting from paused to continuous
 and back again.
</pre>    
### USER CREDIT INFORMATION
<pre>
 Typing =c will tell you information about your account and how many credits
 you have, your current class, date of last log-on, etc.

 Typing =c <user-id> displays account information such as class, credits and
 date of last call on <user-id>.
</pre>    
### WHO'S ONLINE
<pre>
 Typing # will tell you who is online and where, along with other useful
 information. Much more detailed than /#. You can choose whether or not your
 age will be displayed to other users with this command. To allow others to see
 your age, type # ON, while to prevent others from seeing your age, type # OFF.
 You can also select up to 5 different Who's Online displays to see, which vary
 as to what they tell you; for example, some tell the user's age and baud,
 while others don't. You can set your default display by typing # SET <#>,
 where <#> is the number of the default you want from 1 to 5. You can also view
 a display which is different from your default at any time by typing # <#>,
 where <#> is the display you want to see.
</pre>    
### SHOW DATE AND TIME
<pre>
 Typing =dt will give you the current date and time.
</pre>    
### REPEAT PREVIOUS COMMAND
<pre>
 Typing =r will will repeat whatever command you last typed. Please note that
 some commands, like the /p page user command, are used by the BBS before
 Globals can see them, so they cannot be repeated since Globals has no way to
 know about them. You can also type more input after the repeat command to have
 both the last command and your extra input added together to make a new
 command. For example, if your command string is =r, and your last input was
 /go, you could type =r top and your new input would be /go top.
</pre>    
### GLOBAL CHAT CHANNELS
<pre>
 The global chat channels are a way for you to communicate with others on the
 BBS. It is a lot like communicating on a CB radio; you select a channel, and
 whatever you broadcast over that channel is received by anyone else on the
 same channel. The general command is =cc. All chat channel commands are listed
 below. The highest available channel is set so that only those with the proper
 key can use it, and is the Sysop channel.
    
    =cc display             Shows each user's chat channel (for
    			Sysops only).
    
    =cc topic <topic>       Sets your chat channel's topic to
    			<topic>, or if no <topic>, clears
    			the channel's topic. You can select
    			a topic of up to 29 characters.
    
    =cc echo                Toggles echo of chat channel output.
    			ON will display what you send on your
    			chat channel back to you, while OFF
    			will not.
    
    =cc set <channel>       Sets your chat channel to <channel>.
    			Setting channel to 0 turns off global
    			chat messages, while only Sysops may
    			use the highest available channel.
    			Also shows the channel topic and all
    			users on the channel. Note: there may
    			be a short delay between channel changes
    			for non MASTER key holders.
    
    =cc show                Shows all chat channels in use and
    			their topics, as well as how many
    			users on-line are using this
    			channel.
    
    =cc ?                   Shows your current chat channel and
    			all users on your channel as well as
    			the channel topic.
    
    =cc notice              Toggles your notices when users
    			join or leave your global chat
    			channel on and off.
    
    =cc clear               Clears all global chat channel
    			topics.
    
    =cc clear #             Clears global chat channel # topic.
    
    =cc clear # #           Clears all global chat channel topics
    			in the range of channel # to channel
    			#.
    
    =cc <output>            Whatever you type in <output> is
    			displayed to everyone else on your
    			chat channel.
    
    Type =? =cc or =? topic or =? display or =? show or =? set or
    =? echo or =cc clear for help on global chat channels.
</pre>    
### TELEGRAMS
<pre>
 Telegrams are like a quick and easy to use Email. You can think of them as
 being a quick note messaging system. Typing =<userid> <message> will send a
 telegram to <userid>. The telegram will contain <message>. Please note that
 you need to be a little careful when you make a telegram, because under
 strange circumstances the message may not go where you want it to. For
 example, suppose you have two users on your system, Red and Red Knight. If you
 enter =red knight takes your queen, Red Knight will get a telegram saying
 takes your queen instead of Red getting a telegram saying knight takes your
 queen. This will rarely be a problem but please keep it in mind.

 You can type == to see any telegrams people have written to you. If you have
 any telegrams, you will be shown the oldest telegram and can then reply to the
 person who sent it, see the next one, re-display your telegram (if your screen
 gets messed up), or exit viewing your telegrams. Telegrams are automatically
 deleted after you see them if you reply and send the reply without exiting, or
 if you choose to go to	the next telegram.

 You are also notified if you are online when you receive telegrams. If you are
 "Busy" when you receive a telegram, you will be notified when you are no
 longer "Busy". You can toggle telegram notices on and off by typing =ta. You
 can find out what your current setting is by typing =ta ? or use the Current
 account settings global command. Telegram notices from Sysops are	displayed
 regardless of your notification setting.

 You can block up to 5 people from sending you telegrams. Typing =tb <user-id>
 blocks/unblocks <user-id> from sending you telegrams. Note that while people
 you have blocked will not be able to send you telegrams, they can still choose
 to reply to any that you send them. Also note that you may not be able to
 block some users, such as Sysops, from sending you telegrams.

 You can delete telegrams from a specified person without having to read them.
 Typing =td <user-id> will delete any telegrams you have from <user-id>.
 Telegrams from some users, such as Sysops, can not be deleted this way. You
 may experience a delay when you do this, as it could take some time to delete
 every telegram if this person sent you a lot of them.

 Typing =tx <user-id> will delete all telegrams from <user-id> in the telegram
 database. Please be extremely cautious with this command. Not only does it
 remove the telegrams, but it may take a lot of time if the person specified
 has written a lot of telegrams.

 Type =? == or =? = or =? =ta or =? =td or =? =tx or =? =tb for help on
 telegrams.
</pre>    
### MACROS
<pre>
 There are 10 macros available. Macros allow you to type a short string, for
 example =m0, and have your output become a larger one, for example /go
 teleconference. Macros have a command string (the short string) and a macro
 string (the larger string) that correspond to each other, so typing the first
 macro command string would put the first macro in your input. In the example
 above, you would type =m0 and your input would become /go teleconference. The
 command strings to call the macros can be changed by each user to whatever he
 desires up to 4 characters long. The macros of course can also be defined by
 each user and can be up to 39 characters long. To list your current macro
 commands type =mcl. To list your current macros type =mml. To change a macro
 command string, type =mc # <string>, where # is the number of the macro string
 (from 0 to 9) you want to change to <string>. To change a macro, type =mm #
 <string>, where # is the number of the macro (from 0 to 9) you want to change
 to <string>. To use a macro just type its command string. Defaults are =m0,
 =m1, ...=m9. Default macros are blank. You can also add more input to the end
 of a macro by placing the input on the end of the macro command string. For
 example, assume your first macro command string is =m0 (the default) and your
 first macro has been set to /go. You could type =m0 top to go to the main menu
 this way, or =m0 exit to log-off. Please note that macro command strings must
 be only one word.

 Type =? =m or =? =mcl or =? =mml or =? =mm or =? =mc for help on macros.
</pre>
### TRANSFER CREDITS
<pre>
 Typing =trc <user-id> [amount] transfers the amount of your credits you select
 to the user-id you choose. You may transfer up to 9999999 of your credits to
 another user per transfer. There may be a minimum amount of credits you have
 to transfer. Sysops, however, may transfer any amount they have without
 restriction. Some users may not have access to receive credit transfers.
</pre>    
### TRANSFER DAYS IN CLASS
<pre>
 Typing =trd <user-id> [days] transfers the number of your days in class you
 select to the user-id you choose. The user-id you choose MUST have the same
 class you do in order for the transfer to work. You may transfer up to 999 of
 your days to another user per transfer. There may be a minimum number of days
 you have to transfer. You must keep at least one day in class for yourself.
</pre>    
### SHOW ACCOUNT SETTINGS
<pre>
 Typing =cs displays information about your account settings such as ANSI mode,
 editor type, and time left on-line today, as well as your Globals settings
 such as whether to "pester" you in the Editor.

 Typing =cs <user-id> displays information about another user's account and
 Globals settings.

 Note: The By Globals setting tells whether the invisibility setting was set by
 Globals. This is helpful in determining which command to use to toggle
 invisibility back since there are at least two invisibility commands
 available; the /invis command that comes standard with the BBS, and the =in
 command in Globals. If the Globals cannot determine whether an off-line user
 is invisible by another module it will display '?'. This means that the user
 has not been set to be made invisible in their account data the next time they
 log on, and that the Globals will not set them invisible too, but that another
 module could do this.

 Also, a user who is not on-line cannot be "Busy".
</pre>    
### POST CREDITS TO A USER
<pre>
 Typing =pc <user-id> [credits to post] [P/F] (+) will post credits to <user-
 id>. P is PAID credits and F is FREE credits, and (+) optionally informs the
 user of the posting.

 Non MASTER key holders may be limited in the amount they can post with one use
 of the command, while the limit for MASTER key holders is between -9999999 and
 9999999 credits.
</pre>    
### BOOT USER OFFLINE
<pre>
 Typing =zap <user-id to boot> (+) will boot <user-id> off the BBS, with the
 optional (+) telling <user-id> who booted him. Users in Opchat may not be
 booted.
</pre>    
### EXTEND TIME ONLINE
<pre>
 Typing =te <userid> [minutes] (+) will allow you to extend the on-line time of
 <userid> by [minutes] minutes. You can enter a negative number to remove time
 from a user, and minutes can be from -999 to 999. However, you can't give a
 user more than the maximum minutes per day with this command. It probably goes
 without saying that you can't set a user's time on-line to a negative number.
 If the user is on-line, their time for the current call will extended along
 with their time for the day. The optional (+) will inform the user who
 extended his time.
</pre>    
### LIST NEW SIGNUPS
<pre>
 Typing =ln [MM/DD/YY] with the date being optional, will show you new user
 sign-ups for today (if no date given) or for the date you choose, as well as
 showing the city and state the new users are calling from.
</pre>    
### LOG-ON NOTIFIER
<pre>
 Log-on Notifier alerts you to log-ons and log-offs and tells them how many
 users are on-line. It also allows you to make a log-on and log-off message of
 your own to be displayed to others when you log-on and log-off the system. To
 get help on Log-on Notifier you type =log. This will also display your current
 notice setting, ON or OFF. When the setting is ON you will be notified when
 users log-on or log-off, while turning it OFF ends your notifications. You can
 toggle your notice setting by typing =log. To set or change your log-on
 message to others you type =lon [message]. You can clear your current log-on
 message by typing =lon. To set or change your log-off message to others you
 type =loff [message]. You can clear your current log-off message by typing
 =loff. These messages can be up to 39 characters in length. To view your
 current log-on and log-off messages you type =lshow.

 Users with the proper key can view and delete the messages of other users. To
 view another user's messages you type =lshow <user-id>. To delete his messages
 you type =lclear <user-id>.
</pre>    
### SYSTEM-WIDE BROADCAST
<pre>
 Typing =! [message] will broadcast [message] to all users on-line who can
 receive messages (their "Busy" flag is not set).
</pre>    
### POST DAYS IN CLASS
<pre>
 Typing =pd <user-id> [number of days] (+) will post [number of days] days in
 class to <user-id>. Allowable values for [number of days] are -999 to 999. It
 should be obvious that the user's class must use days in class to do this.
 Adding the optional (+) will inform the user of the posting.
</pre>    
### SWITCH USER CLASS
<pre>
 Typing =sc <user-id> [class name] [days in class] (+) will switch <user-id> to
 class of [class name], for [days in class] days. [days in class] is optional,
 and if you do not include it, the	class switch will be for a default number of
 days if the new	class uses days in class. Valid numbers for [days in class]
 are 1 to 999. Note that you may not always get the change you want, depending
 on the circumstances. For example, trying to switch a "DEMO" user without
 credits to "PAYING" class will not work. Also, switching a "MONTHLY" user with
 credits to "DEMO" class will switch the user to "PAYING" until his credits are
 consumed, and then the user will be switched to "DEMO".

 The (+) is optional and will inform the user of who switched his class.
</pre>    
### GIVE OR TAKE USER KEY
<pre>
 Typing =uk <user-id> [key] (+) will give [key] to <user-id> if he doesn't have
 [key], or remove it from him if he does have it. The (+) is optional and will
 inform the user that the key was given or taken.

 You can list a user's current keys by typing =uk <user-id> ?. Note that you
 can remove keys in a user's class with this command. The keys will still show
 up when you list the user's class keys, but the keys will also show up as
 being negative keys. This means the user technically has the key in his class
 keyring, but he won't be able to use the key, so effectively it removes the
 key from the user. Please be aware that it is probably not a good idea to
 remove keys like NORMAL from a user this way, and that you should only take
 away special keys from a user's class.
</pre>    
### INVISIBILITY TOGGLE
<pre>
 Typing =in will toggle invisibility on and off. Normal users cannot tell that
 an "invisible" person is on the system. You can also toggle invisibility on
 and off for other users. This is done by typing =in <user-id> (+), where
 <user-id> is the user you wish to toggle invisibility for. The (+) is optional
 and informs the user who has toggled his invisiblity.

 The invisibility lasts until it is toggled off. Users who can become invisible
 can also see others who are invisible on-line by using the Who's Online
 command if they have the key to toggle invisibility. However, it is up to the
 Sysop whether they can see MASTER key holders who are invisible.

 Only users with the MASTER key may toggle the invisibility of other MASTER key
 holders.
</pre>    
### SUSPEND USER WITH AUTOMATIC UNSUSPEND
<pre>
 Typing =sa <user-id> # (+) will suspend <user-id> for # days, where days can
 be from 1 to 365. After # days, <user-id> will be automatically unsuspended.
 The user will be booted from the system if he is on-line. You can see all
 users who are suspended and set to be automatically unsuspended, along with
 their automatic unsuspend dates, by typing =su ?.

 The optional (+) informs an online user who has suspended him.

 If you suspend a user with this command who has already been suspended with
 this command, the newest automatic unsuspend date is the one which is used.

 To unsuspend a user who has been suspended with this command, use the suspend
 toggle command by typing =su <user-id>. MASTER key holders may not be
 suspended.
</pre>    
### SUSPEND/UNSUSPEND USER TOGGLE
<pre>
 Typing =su <user-id> (+) will suspend <user-id> if he is not already
 suspended; otherwise, it unsuspends <user-id>. If <user-id> is suspended he is
 also booted off the system if he is on-line.

 The optional (+) informs an online user who has suspended him.

 MASTER key holders may not be suspended.
</pre>    
### DELETE/UNDELETE USER TOGGLE
<pre>
 Typing =ku <user-id> (+) will delete <user-id> if he is not already deleted;
 otherwise, it undeletes <user-id>. If <user-id> is deleted he is also booted
 off the system if he is on-line.

 The optional (+) informs an online user who has deleted him.

 MASTER key holders, and those accounts that are PROTECTED may not be deleted.
</pre>    
### PAGE USER
<pre>
 Typing =p <user-id> [message] will send a page to a user, along with the
 message if any. You can have three page settings; ON, which means you can
 receive unlimited pages, OK, which means you can be paged every two minutes,
 and OFF, which means you won't receive pages at all. Typing =p ? will give you
 your current page setting. Typing =pr repeats the last page you sent. Typing
 =pf <user-id> blocks/unblocks that user from sending you pages. Typing =pf ?
 shows you who you've blocked from sending you pages. You can block up to 5
 users from sending you pages. You will not be able to block some users such as
 Sysops from paging you, and these users will also be able to page you even if
 you set your page setting to OFF or OK at any time.

 If a user is "Busy" when you page them, they will be sent a telegram with your
 message instead of a page. Note that Sysops may be able to page you whether
 you are "Busy" or not, and even if your pages are turned OFF. Type =? =p or =?
 =pf or =? =pr for help on paging.
</pre>    
### COLORIZE INPUT
<pre>
 You can add color to what you type with this global command.

 You can color the both the text and the background, as well as create special
 effects like blinking text. The colors available are:
    
    0 Black
    1 Red
    2 Green
    3 Yellow
    4 Blue
    5 Magenta
    6 Cyan
    7 White

 Special effects available:
    
    0 Normal output
    1 Bold output (default)
    2 Blinking text
    3 Reverse video (light background and dark letters)

 To color your input text, you would put the following command string in front
 of the input you want colored:

 \f#    Colors your text, where # is the color (0-7) To color the background of
 your input, you would put the following command string in front of the input
 you want colored:

 \b#    Colors your background, where # is the color (0-7) To use a special
 effect, you would put the following command string in front of the input you
 would like the effect in:

 \s#    Gives a special effect, where # is effect (0-4) It may help you to
 remember the commands by thinking that f stands for foreground (text), b
 stands for background, and s stands for special. An example of how to use
 color: =sysop \f1\s1Dude!!!

 This will send a telegram to Sysop with red blinking text that says "Dude!!!".

 Note that you can get more colors by using mixing the use of the bold output
 and normal output special effects when you colorize.

 Also note that colorizing your input may result in much longer input, and you
 may be restricted accordingly. For example, you can have macros of up to 39
 characters, but if you colorize some of this input, you will not be allowed to
 have 39 colorized characters. This is because the colorizing takes up some of
 the available space.
</pre>    
### EDITOR PESTER TOGGLE
<pre>
 This option allows you to notify the Globals that you don't want to be
 bothered while you are in the Editor. This will allow you to be in the Editor
 and not be annoyed by log-on notices, pages, telegrams, and so on. However, as
 soon as you leave the Editor you will receive these messages automatically.
 Turning your setting ON will allow you to receive these messages in the
 Editor, while turning it OFF will prevent you from being pestered. Typing
 =pest toggles your setting.

 Please note: Only those messages from Globals will be prevented from being
 sent to you with this command. For example, the =p global paging command will
 not bother you if you turn your setting OFF, but the /p global paging command
 that comes with the Teleconference module in the BBS will still reach you.

 Also note that Sysops will be able to page you while you are in The Editor
 even if you turn your Pester toggle OFF.
</pre>    
### ALARM
<pre>
 This command allows you to set a time at which you wish to be notified what
 time it is, much like you set your alarm clock to ring in the morning to wake
 you up. You can use this command to notify you of when you need to leave or
 run out of time, when certain events in games occur, etc.

 To set an alarm time, you just type:
    =* <HH:MM:SS>

 where HH is the hour, MM is the minutes, and SS is the seconds in 24 hour
 format. For example, 1:00:00 is 1 AM, while 13:00:00 is 1 PM.

 To display your current alarm setting, you type:
    =* ?

 To clear your alarm setting, you type:
    =*

 Note that if you are "Busy" when your alarm is set, it will alert you as soon
 as you are no longer "Busy", so you can still get your alarm.
</pre>    
### FRIENDS NOTICE
<pre>
 This command notifies you when a user you choose logs onto or off of the BBS.
 If you are "Busy" when the Friends you choose log on or off, then you are
 notified as soon as you are no longer "Busy". To set your Friends
 Notifications, you type:

    =+ ON <User-ID> 	Notifies you when <User-ID> logs on
    =+ OFF <User-ID>	Notifies you when <User-ID> logs off

 To see your current Friends Notices, you type:
    =+ ?

 To clear your current Friends Notices, you type:
    =+ ON			Clears your Friends Notice log on
    =+ OFF			Clears your Friends Notice log off

 The Notices you choose are good only for as long as you are on-line, so when
 you log off you will need to reset your Friends Notices.
</pre>
## INSTALLATION AND CONFIGURATION
  
 Simply unzip the archive to your BBS server directory, add
 it to your menu, configure the MSG file to your liking, and start
 the BBS!  It's that easy!

 LEVEL3 has a number of key-configurations for various commands. Pay close 
 attention to the commands that should be for SUPER users only.
 
 LEVEL4 allows you to set the global command strings for the various commands
 as well as configurations for things like sysop chat channel, beeps for the
 console page, number of chat channels, and more. 
 
 LEVEL6 has numerous text blocks that can be configured!

## MODULE HISTORY

 Globals was written in December 1994 by "Easy Ed" Erdman of EzSoft. The
 module was available up through Worldgroup 3.0. EzSoft was acquired by
 AdventureComm, which was acquired by Elwynor Technologies in 2004.

 Elwynor Technologies ported the module to Worldgroup 3.2 in May 2021, and
 to The Major BBS V10 in August 2024.
		
 
## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for 
 more details.

## CONTRIBUTING

 We welcome contributions from the community. By contributing, you agree to the
 terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be licensed 
 under the AGPL v3, assign all right, title, and interest, including all 
 copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and 
 you must include the additional terms from the LICENSE file in your project's 
 LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWGLOB" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWGLOB.ZIP. When unzipped in a Worldgroup 3.2 or Major BBS V10 installation 
 folder, it "installs" the module.
