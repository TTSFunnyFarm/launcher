# Toontown's Funny Farm Launcher
https://www.toontownsfunnyfarm.com/

## Installation Guide

### Setting up Visual Studio 2019

Download the Visual Studio 2019 Installer by downloading it from https://visualstudio.microsoft.com/vs/. Any edition is fine, but this tutorial is based off of my experience using Visual Studio Community 2019, which is the free version.

Note: Once you run the installer, it will download the files needed to give you advanced set-up options when installing Visual Studio 2019.

Make sure that 'Desktop development with C++' is ticked. This will then produce a list of features that will be installed upon hitting the Install button.

At the bottom of this new list, tick both the "MSVC v141..." and "MSVC v140..." options. These do have more to them, hence the ellipses. I'd just rather not type them out.

Click the install button to finish installing Visual Studio Community 2019 with the adjustments you have made to the installation. Restart your computer when it asks you to post-installation.


### Installing Qt

Once you've done that, install Qt by downloading it from the website here: https://www.qt.io/download. In this tutorial I am using the free open-sourced version of Qt, but the commercial one may work if needed.

Login to your Qt account when asked to by the installer, or skip this step if you'd rather not have an account.

Click next, and then install Qt to the root of your drive. This should be the default installation folder Qt presents to you, so don't change it.

On the 'Select Components' screen, expand the list for 'Qt 5.12.3' and ensure that the following is ticked...

- MSVC 2015 64-bit
- MSVC 2017 32-bit
- MSVC 2017 64-bit
- Qt WebEngine
- Qt Network Authorization
- Qt Debug Information files

...and then expand the list for 'Developer and Designer Tools' and ensure that the following is ticked...

- Qt Creator 4.9.1 CDB Debugger Support
- OpenSSL 1.1.1b Toolkit

...and click next. Alternatively, you could just install all of Qt 5.12.3, as well as all of the Developer and Designer Tools, but this tutorial aims to save you space so that you can specifically open the Toontown's Funny Farm launcher developer build.

Read and agree to the license agreements presented to you, and click next. Leave the start menu folder as 'Qt' and hit install.

### Running the launcher

Once Qt has downloaded and installed, download the launcher files from this GitHub repo either as a zip file or via GitHub Desktop. I am going to be using GitHub Desktop.

Once you've pulled the latest origin, open the 'FFLauncher.pro' file in Qt. You should see a screen that says 'Configure Project'.

Hover over the 'Desktop Qt 5.12.3 MSVC2015 64bit' kit and click 'Manage...'. You will then be greeted with a screen listing your installed 'kits'.

Click 'Desktop Qt 5.12.3 MSVC2017 64bit' in the kits list and and then change the following...

### Compiler
- C: from "<No compiler>" to "Microsoft Visual C++ Compiler 16.1.28922.388 (amd64)"
- C++: from "<No compiler>" to "Microsoft Visual C++ Compiler 16.1.28922.388 (amd64)"

...and then click OK to apply the changes.

Once you have done that, untick the 'Desktop Qt 5.12.3 MSVC2015 64bit' kit and then make sure the new kit listed, 'Desktop Qt 5.12.3 MSVC2017 64bit', is ticked.

Finally, click 'Configure project'.

You should now be able to run the developer build of the Toontown's Funny Farm launcher by using the green play button in the bottom left of your screen.

Once you click the play button, give your computer time to compile an on-the-fly version of the launcher that can be viewed and used as intended. It will proceed to launch automatically.
