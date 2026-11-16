---
name: Bug report
about: Create a report to help us fix an issue in MControlCenter Extra
title: '[Bug]: '
labels: 'bug'
assignees: ''
---

**Laptop Model & Board**
e.g., MSI Modern 14 C7M (MS-14JK)

**OS / Distribution & Kernel**
e.g., Debian 13 (Trixie), Kernel 6.12.x

**MControlCenter Version**
e.g., 0.5.2 (from deb, tarball, or built from source)

**Describe the bug**
A clear and concise description of what the bug is.

**Steps to Reproduce**
1. Open MControlCenter Extra
2. Go to '...'
3. Click on '...'
4. See error

**Terminal Output / Logs**
Run `mcontrolcenter` in a terminal and paste output here:
```text
paste terminal output here
```

**Driver status**
- Output of `cat /sys/devices/platform/msi-ec/shift_mode`:
- Is `ec_sys` or `acpi_ec` loaded with `write_support=1`?
