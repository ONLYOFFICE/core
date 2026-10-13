//go:build !windows

package main

import "os"

func replaceFile(source, target string) error { return os.Rename(source, target) }
