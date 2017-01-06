#!/bin/sh 

echo "Getting updated ban file from pisto's github..."

wget https://raw.githubusercontent.com/pisto/ASkidban/master/compiled/ipv4 -Oaskidbans.cfg

echo "Editing  file to fit remod regular ban file..."

sed -i -e 's/^/askidban /' askidbans.cfg
sed -i -e 's/$/ "askidban"/' askidbans.cfg

echo "Done! Now NSA will not connect to your server."

