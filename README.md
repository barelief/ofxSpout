ofxSpout
========

### info

V2.001-beta
http://spout.zeal.co/
source: https://github.com/leadedge/Spout2

## troubleshooting

### Intel videocard issues
add this line in `ofApp::setup()` after `spoutsender = new SpoutSender`:

```
spoutsender->SetDX9(true);  // in SpoutSender example
// or
spoutreceiver->SetDX9(true); // in SpoutReceiver example
```
thread: http://spout.zeal.co/forums/topic/createinterop-error-3/

## meta

tested on win7, win8, nvidia 5xx, intel hd 5000
