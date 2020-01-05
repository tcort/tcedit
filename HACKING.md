# Hacking

## Preparing a release

### Copyright Year

Is the current year in the copyright header? If not, add it.

### Version Bump

Bump version in `configure.ac`

### Update CHANGELOG.md

Write about what has changed since the last release.

### Tag

    git tag -a v2019.12.25 -m "Version 2019.12.25"
    git push && git push --tags

### Build Release Tarball

    git clone git://github.com/tcort/tcedit && cd tcedit
    ./autogen.sh
    ./configure
    make distcheck

### Create release on github

[Draft a new release](https://github.com/tcort/tcedit/releases/new)

### Upload

[Upload release tarball](https://github.com/tcort/tcedit/releases)

### Update Release Notes

Set notes to something like this:

```
Source package with `configure` script: [tcedit-2019.12.25.tar.gz](https://github.com/tcort/tcedit/releases/download/v2019.12.25/tcedit-2019.12.25.tar.gz)

Notable changes:

* Initial Release
```
