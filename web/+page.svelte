<script lang="ts">
	import { onMount } from 'svelte';
	import Program from './dungeon';

	let Module: any;
	let textConsole: HTMLPreElement;

	function onKeydown(e: KeyboardEvent) {
		if ('wasd'.includes(e.key.toLowerCase())) {
			e.preventDefault();
			e.stopPropagation();
			if (!textConsole.style.minHeight) {
				textConsole.style.minHeight = `${textConsole.clientHeight}px`;
			}
			textConsole.innerText = '';
			Module.frame(e.keyCode);
		}
	}

	onMount(() => {
		Program({
			print(text: string) {
				textConsole.innerText += `${text}\n`;
			},
			locateFile(wasmBinaryFile: string) {
				return `/assets/${wasmBinaryFile}`;
			}
		}).then((module: any) => {
			Module = module;
			Module.frame(87); // keycodes?
			textConsole.innerText = '';
			Module.frame(83);
			document.addEventListener('keydown', onKeydown);
		});
	});
</script>

<div class="flex flex-col items-center grow gap-5 light-on-dark p-5">
	<h1>Dungeon crawler</h1>
	<p class="m-0 rounded p-1 -mt-5">You can try out the web demo with limited features:</p>
	<pre bind:this={textConsole} class="border border-black inline-block p-2" />
	<div class="rounded p-2 max-w-[600px] w-full text-center flex flex-col items-center">
		<p>
			Made in C++. Web version compiled with <a href="https://emscripten.org/" class="link"
				>Emscripten</a
			>.
			<br />Use WASD to control.
		</p>
		<p>
			<a
				href="https://github.com/discapes/dungeoncrawler/releases/download/1.0/dungeoncrawler.exe"
				class="link">Windows download</a
			>. Thumbnail:
		</p>
	</div>
	<p class="dark-on-light round p-2">
		<a href="/projects#dungeon" class="link">Go back</a>
	</p>
</div>
