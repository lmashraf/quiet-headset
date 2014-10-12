quiet-headset
=============

QuietHeadset is a Windows Service that uses MMDevice API to automatically lower or mute the master volume if the audio endpoint device is unplugged.


How to use:
===========
In order to install as a windows service, run the following command:

```sc create "Quiet Headset Service" binPath= "path_to_exe_file"```

To remove the service:

```sc delete "Quiet Headset Service"```
