# cdeps — C Dependencies Manager

[![GitHub release](https://img.shields.io/github/release/danidomi/cdeps.svg)](https://github.com/danidomi/cdeps/releases)

`cdeps` is a tiny command-line tool that fetches prebuilt C release artifacts
from GitHub releases, picks the right zip for the current platform, and drops
the contents under `deps/<repo>/` so your project's compiler can `-Ideps` and
link against them.

It's the consumer-side counterpart to
[crels](https://github.com/danidomi/crels), which produces the per-platform
zips that `cdeps` downloads.

## Table of Contents

- [Installation](#installation)
- [Quick start](#quick-start)
- [Commands](#commands)
- [The `c.deps` file](#the-cdeps-file)
- [How it resolves a dependency](#how-it-resolves-a-dependency)
- [Contributing](#contributing)
- [License](#license)

## Installation

```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/danidomi/cdeps/HEAD/install.sh)"
```

The installer drops a `cdeps` binary under `~/.local/bin/`. Make sure that's
on your `PATH`:

```shell
export PATH="$HOME/.local/bin:$PATH"
```

## Quick start

Create a `c.deps` file in your project root listing your dependencies, one
per line:

```
github.com/danidomi/c-framework-service v0.4.0
```

Then:

```shell
cdeps install
```

`cdeps` looks at the file, downloads the platform-matched release zip for
each entry from GitHub, and unpacks it under `deps/<repo>/<OS>_<ARCH>/`.
You'll typically flatten that one more level so your `-Ideps` resolves the
includes at the natural path:

```shell
for d in deps/*/Linux_* deps/*/Darwin_*; do
    [ -d "$d" ] || continue
    mv "$d"/* "$(dirname "$d")/"
    rmdir "$d"
done
```

(The example services do this in their Dockerfile.)

## Commands

### `cdeps install`

With no argument, reads `c.deps` and installs every entry:

```shell
cdeps install
```

Or install a single dependency directly without a `c.deps` file:

```shell
cdeps install github.com/danidomi/c-framework-service@v0.4.0
cdeps install github.com/danidomi/c-framework-service@latest
```

### `cdeps list`

Print the contents of the project's `c.deps` file.

```shell
cdeps list
```

### `cdeps clean`

Remove `deps/` and any cached download artifacts.

```shell
cdeps clean
```

### `cdeps version`

Print the installed `cdeps` version.

```shell
cdeps version
```

## The `c.deps` file

One dependency per line, format:

```
<host>/<owner>/<repo> <version>
```

Example:

```
github.com/danidomi/c-framework-service v0.4.0
github.com/example/libfoo              latest
```

**Versions need the `v` prefix** when the upstream repo tags releases that
way (e.g. `v0.4.0`). `cdeps` interpolates the version string verbatim into
the GitHub release URL — if your tags are `v0.4.0`, you must write `v0.4.0`
in `c.deps`. The bare keyword `latest` resolves to the most recent release.

## How it resolves a dependency

For an entry `github.com/owner/repo v1.2.3`, `cdeps` GETs:

```
https://github.com/owner/repo/releases/download/v1.2.3/<OS>_<ARCH>.zip
```

— where `<OS>` and `<ARCH>` come from `uname`. For `latest`:

```
https://github.com/owner/repo/releases/latest/download/<OS>_<ARCH>.zip
```

For example, on a Linux x86_64 host with `c-framework-service v0.4.0`:

```
https://github.com/danidomi/c-framework-service/releases/download/v0.4.0/Linux_x86_64.zip
```

The release zip is expected to contain a per-platform directory
(`<OS>_<ARCH>/`) with the prebuilt object file plus copies of the headers.
That layout is what `crels` produces by default.

## Contributing

Issues and PRs welcome.

## License

MIT.
