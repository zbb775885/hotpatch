/*
 * hotpatch is a dll injection strategy.
 * Copyright (c) 2010-2011, Vikas Naresh Kumar, Selective Intellect LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *	 notice, this list of conditions and the following disclaimer in the
 *	 documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Selective Intellect LLC nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <hotpatch_config.h>
#include <hotpatch_internal.h>
#ifdef HOTPATCH_HAS_ASSERT_H
	#undef NDEBUG
	#include <assert.h>
#endif

int main(int argc, char **argv)
{
    struct ld_procmaps *maps = NULL;
	size_t mapnum = 0;
	int ret;
	pid_t pid = argc > 1 ? (pid_t)strtol(argv[1], NULL, 10) : getpid();
	assert(pid != 0);
	maps = ld_load_maps(pid, 6 /* largest verbose */, &mapnum);
	assert(mapnum > 0);
	assert(maps != NULL);
	ret = 0;
	ret = ld_find_library(maps, mapnum, "[heap]", false, NULL, 6);
	assert(ret >= 0);
	ret = ld_find_library(maps, mapnum, "[stack", false, NULL, 6);
	assert(ret >= 0);
	ret = ld_find_library(maps, mapnum, "vdso", false, NULL, 6);
	assert(ret < 0);
	ret = ld_find_library(maps, mapnum, "libc", false, NULL, 6);
	assert(ret >= 0);
#if __WORDSIZE == 64
	ret = ld_find_library(maps, mapnum, "/lib64/ld-linux-x86-64.so.2",
						  true, NULL, 6);
	assert(ret >= 0);
	ret = ld_find_library(maps, mapnum, "/lib/ld-linux-x86-64.so.2",
						  false, NULL, 6);
#else
	ret = ld_find_library(maps, mapnum, "/lib/ld-linux.so.2",
						  true, NULL, 6);
	assert(ret >= 0);
	ret = ld_find_library(maps, mapnum, "/lib32/ld-linux.so.2",
						  false, NULL, 6);
#endif
	assert(ret < 0);
	ld_free_maps(maps, mapnum);
    return 0;
}

