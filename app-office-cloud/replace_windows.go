//go:build windows

package main

import (
	"syscall"
	"unsafe"
)

var moveFileEx = syscall.NewLazyDLL("kernel32.dll").NewProc("MoveFileExW")

func replaceFile(source, target string) error {
	s, e := syscall.UTF16PtrFromString(source)
	if e != nil {
		return e
	}
	t, e := syscall.UTF16PtrFromString(target)
	if e != nil {
		return e
	}
	r, _, err := moveFileEx.Call(uintptr(unsafe.Pointer(s)), uintptr(unsafe.Pointer(t)), uintptr(0x1|0x8))
	if r == 0 {
		return err
	}
	return nil
}
