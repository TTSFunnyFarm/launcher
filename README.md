# Toontown's Funny Farm Launcher
https://www.toontownsfunnyfarm.com/

## Installation Guide

### Setting up Visual Studio 2019

Download the Visual Studio 2019 Installer by downloading it from https://visualstudio.microsoft.com/vs/. Any edition is fine, but this tutorial is based off of my experience using Visual Studio Community 2019, which is the free version.

Note: Once you run the installer, it will download the files needed to give you advanced set-up options when installing Visual Studio 2019.

Make sure that 'Desktop development with C++' is ticked. This will then produce a list of features that will be installed upon hitting the Install button.

At the bottom of this new list, tick both the "MSVC v141..." and "MSVC v140..." options. These do have more to them, hence the ellipses. I'd just rather not type them out.

Click the install button to finish installing Visual Studio Community 2019 with the adjustments you have made to the installation. Restart your computer when it asks you to post-installation.


### Setting up Qt

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

