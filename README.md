# SvenDPP
This plugin, similar to the [gus](https://github.com/incognico/gus) plugin, allows your discord server to communicate with the `Sven Co-op game server`. Unlike [gus](https://github.com/incognico/gus), this plugin can be used on any of your servers.<br><br>
This plugin is not the `final version`, it is not `multifunctional` and has `many problems` including game crashes. This is only a test plugin at the moment.<br><br>
This plugin is so called because it uses the `DPP` (`Discord Plus Plus`) library.<br>
I will finalize this plugin when I have free time.

# Installation
Installing the plugin consists of several steps:
1. Download the [release](https://github.com/kekekekkek/SvenDPP/releases/download/v0.1/SvenDPP.rar) version of the plugin;
2. `Unzip` the files from the archive to a convenient location;
3. Go to the root folder of the game (for example `C:\Program Files (x86)\Steam\steamapps\common\Sven Co-op`) and move all modules from the `Sven Co-op` folder to the root directory of the game. Remember, never rename the name of modules (`.dll`);
4. Now you will need to `create` your `bot` in discord and add it to your server. Go to this [link](https://discord.com/developers/applications) and click the `New Application` button;<br><br>
![Screenshot_1](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_1.png)<br><br>
5. After creating the bot, you will need to go to the `Bot` tab;<br><br>
![Screenshot_2](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_2.png)<br><br>
6. Here you will need to click on the `Reset Token` button. After resetting the token, you will need to `copy` it and `save` it to some `text file`. The token will look something like this: `MFEA8U312NjAAJGHFEAFJ11NFG813421QzNTM2MzM2OA.GYRvA8.v-cOI6F0qO412fahfeaJRqfToWKuYGITERBYLWI5IM`;<br><br>
![Screenshot_3](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_3.png)<br><br>
7. Now you need to go to the `OAuth2` tab;<br><br>
![Screenshot_4](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_4.png)<br><br>
8. Here you need to find the `OAuth2 URL Generator` section and in it find the `bot` checkbox and click on it. After that, just below in `BOT PERMISSIONS` you need to select the `Administrator` checkbox;<br><br>
![Screenshot_5](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_5.png)<br><br>
9. Now there is a `link` at the bottom of the page that you need to `copy and paste` it into your `browser`;<br><br>
![Screenshot_6](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_6.png)<br><br>
10. After that you will need to `select the server` on which you want to `add the bot` and click on the `Continue` button;
11. Now you need to create or select an existing `channel` and `copy` its link. In the `link`, you will need to `copy` the `last digits` and `save` to the same file where you saved the bot token;<br><br>
![Screenshot_7](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_7.png)<br><br>
![Screenshot_8](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_8.png)<br><br>
12. After these steps, run the `SvenJect.exe` program. In the `Token` text field specify the `bot token`. In the `Channel ID` text field specify the `channel number`. You can do this using the keys `Ctrl + C` and `Ctrl + V`. The result of copy and paste should look like this:<br><br>
![Screenshot_9](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_9.png)<br><br>
13. Now start the game and create your server. You can simply use the command in the console: `map %mapname%`;
14. Once the server has been fully loaded, go to the `SvenJect.exe` program and click on the `Load` button;
15. If you have done everything correctly, then after loading the module into the game process, the bot in discord should send you the message `SvenDPP.dll successfully injected!` and also, the injector should display a window with the same text;
16. Never rename the `SvenDPP.dll` module name. Also `SvenDPP.dll` module must be located in the same directory with `SvenJect.exe` injector;
17. Now `you can send messages` to the `game chat` and they will be displayed on `your server in discord`. Other players messages will also be displayed in the discord server.

# Basic Commands
Currently there is only one `!send` command available to the bot. There are plans to add more commands to interact with the game server in the future.
### [+] Commands:
* `!send %msg%` - Allows you to send messages from your discord to the game server. Messages sent with this command will be displayed to all players on the game server. This is the command you need to write in your discord server. A simple example: `!send Hello from Discord!`.
  
# Compile C++
### [+] SvenJect:
**Build type**: `Release/x86`<br>
**Information**: `SvenJect` uses the standard `gdi32.dll` library to render the control, so you don't need to plug in any dependencies or add additional libraries to your project. The `gdi32.dll` library is always loaded into all processes by default.
### [+] SvenDPP:
**Build type**: `Release/x86`<br>
**Information**: To compile a `SvenDPP` project, you will need to specify the included directories and libraries in your project properties. You will also need to add the `dpp.lib` library to the linker. Details of the installation process and the library used can be found in the `Additionally` section.

# Additionally
### [+] About the module and the injector:
* The injector interface does not use standard controls with the `OWNERDRAW` flag. All interface elements are drawn manually using `double buffer`, but sometimes the interface may flicker. The reason for this is that I am having trouble using the `vgui.lib` library to render the window as `Sven Co-op Dedicated Server` does;
* There is also a `TABSTOP` system in the interface as is done in standard windows;
* It seems to me that drawing the interface manually is much faster than restyling the interface via the `OWNERDRAW` flag, especially since not all standard `Windows` elements support this flag;
* Also, the `SvenDPP.dll` library uses the `OutputDebugString` function hook, so duplicate messages or messages logged by the game may appear on your server in `Discord`;
* Also in the `Discord` server can be output commands that the player uses in his console. The best option is to use the `SayText` function hook, which will only display messages when the player writes something in the chat. This method will be used in the future;
* There are also font display issues on older Windows. The window system menu uses `Unicode` characters as text. For example:<br><br>
![Screenshot_10](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_10.png)<br><br>
The `Load`, `Save` and `Cancel` buttons are used for the following purposes:
* `Load` - used to inject the module into the game process. If your game will not run or there is any error during injection, the program will show you `MessageBox` with the error text;
* `Save` - used to save entered values to text fields. All values are saved in the system registry. Also, field values are saved automatically when you click the `Load` button;
* `Cancel` - used to close the program. You can also close it by pressing the `X` button or the `Escape` key.
### [+] About the library used:
The library that was used is presented below:
1. Library repository - https://github.com/brainboxdotcc/DPP;
2. Library installation - https://dpp.dev/install-windows-zip.html.

**REMEMBER**: This library is not a `final` product and will be finalized in the future.<br>
**REMEMBER**: You may also have frequent crashes when using it.<br>
**REMEMBER**: You may also find `junk code` in the source code, which will be removed in the future.<br>
**REMEMBER**: Also, failures may occur if there is no token or channel id (I have not tested this).<br>
**REMEMBER**: Crashes can occur if you send too large a message to the server, or if a player writes a large message.<br>
**REMEMBER**: Also, `SvenJect.exe` uses only the `main window` for drawing, so other `dialog boxes` currently use the standard interface.<br>
**REMEMBER**: The `text field` has a simple implementation, so it does not have the functionality of standard `Windows` controls.<br>
**REMEMBER**: You can click on the button ![Screenshot_11](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_11.png) to get more information on how to use editbox.<br>
**REMEMBER**: All this functionality could have been done via `Metamod` or `SvenMod`.<br>
**REMEMBER**: You can inject the library manually and without using `SvenJector.exe`. Only I don't know what happens if you try to inject this library into the `svends.exe` process.<br>
**P.S**: Yes, the `OutputDebugString` hook will work locally, since `server.dll` does not import it.

# Screenshots
* Screenshot 1<br><br>
![Screenshot_12](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_12.png)
* Screenshot 2<br><br>
![Screenshot_13](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_13.png)
* Screenshot 3<br><br>
![Screenshot_14](https://github.com/kekekekkek/SvenDPP/blob/main/Images/Screenshot_14.png)
