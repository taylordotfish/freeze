Freeze
======

Version 0.1.0


Dependencies
------------

* LV2 development files
* GTK+ 2 development files
* GCC
* GNU Make

On Debian GNU/Linux (and many derivatives), these can be installed by running
the following command as root (e.g., with ``sudo``):

```
apt-get install lv2-dev libgtk2.0-dev gcc make
```


Installation
------------

Run the following commands (you will need to have [Git] installed):

```
git clone https://git.taylor.fish/git/freeze ~/.lv2/freeze.lv2/
cd ~/.lv2/freeze.lv2/
make
```

[Git]: https://git-scm.com/


License
-------

Freeze is licensed under the GNU General Public License, version 3 or any later
version. See [LICENSE].

This README file has been released to the public domain using [CC0].

[LICENSE]: LICENSE
[CC0]: https://creativecommons.org/publicdomain/zero/1.0/
