#!/bin/bash
set -e

# FIXME: Specify the research branch of linux repository
./gentree.py --clean ../linux/ ../release/

echo "Creating archive ..."
tar -czf ../release.tar.gz ../release/
