ofxSpout
========

## info

http://spout.zeal.co/

https://github.com/leadedge/Spout2

## troubleshooting

if you are having issues on Intel videocard - add this line in *ofApp::setup()* after `spoutsender = new SpoutSender`:

```
spoutsender->SetDX9(true);  // in SpoutSender example
// or
spoutreceiver->SetDX9(true); // in SpoutReceiver example
```
thread: http://spout.zeal.co/forums/topic/createinterop-error-3/

## meta

tested on win7, win8, nvidia 5xx, intel hd 5000
