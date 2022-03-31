# RMT example ported from Kolban's snippets:
https://github.com/nkolban/esp32-snippets/blob/master/rmt/fragments/rmt_simple.c
## Add second RMT channel and test how to adjust both chnannels to start simultaneously
Actually second RMT channel starts lag about 14us. So need add for first channel additional long pause about 14us and after that both channels can starts simultaneously

