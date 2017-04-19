#!/bin/sh

set -e

if [ -d _darcs ]; then
    export DARCS_DONT_ESCAPE_ISPRINT=1
    darcs changes > ChangeLog
elif [ -d .git ]; then
    git log --pretty=tformat:'%aD  %aN <%aE>%n  * %s%n%w(80,4,4)%+b' > ChangeLog
fi

echo po/*.po | sed 's,po/\([^ ]*\)\.po,\1,g' >po/LINGUAS
sed -n '/^\(Hint\)$/,/^[^|]/ s,^| \(.*\)$,_("\1"),p' `find -name '*.level' | grep -v "levels/tests"` >po/leveltexts.txt
sed -n '/^\(Name\)$/,/^[^|]/ s,^| \(.*\)$,_("\1"),p' `find -name '*.level' | grep -v "levels/tests"` >>po/leveltexts.txt
sed -n '/^\(Description\)$/,/^[^|]/ s,^| \(.*\)$,_("\1"),p' `find -name '*.level' | grep -v "levels/tests"` >>po/leveltexts.txt
sed -n '/^\(Tutorial\)$/,/^[^|]/ s,^| \(.*\)$,_("\1"),p' `find -name '*.level' | grep -v "levels/tests"` >>po/leveltexts.txt

autoreconf -i -s
