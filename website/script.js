let colorPicker = new iro.ColorPicker("#picker", {
	borderWidth: 3,
	color: "#ffffff"
});

let gColor

const changeColor = (color) => {
	let col = color.clone();
	gColor = col.hexString;
	col.setChannel("hsl", "l", color.hsl.l / 3);
	let gColDark = col.hexString;

	document.querySelectorAll(".colored").forEach((e) => {
		e.style.backgroundColor = e.classList.contains("dark")
			? gColDark
			: gColor;
	});

	if (next) {
		next = false;
		let req = new XMLHttpRequest();
		req.open(
			"GET",
			`/api?r=${color.rgb.r}&g=${color.rgb.g}&b=${color.rgb.b}`
		);
		req.addEventListener("load", () => {
			next = true;
		});
		req.send();
	}
};

{
	let req = new XMLHttpRequest();
	req.open(
		"GET",
		`/getColor`
	);
	req.addEventListener("load", () => {
		console.log(req.responseText);
		colorPicker.setColors([req.responseText]);
		changeColor(colorPicker.color)
	});
	req.send()
}

let next = true;

colorPicker.on("color:change", (color) => changeColor(color))

let bg = document.querySelector(".bg");

setInterval(() => {
	createCircle();
	createCircle();
	createCircle();
	createCircle();
	createCircle();
}, 1000);

function createCircle() {
	let circle = document.createElement("div");

	circle.classList.add("circle");
	circle.classList.add("colored");

	circle.style.backgroundColor = gColor;

	if (Math.random() > 0.35) circle.classList.add("light");
	else circle.classList.add("dark");

	circle.style.setProperty("--pos-x", Math.floor(Math.random() * 100) + "vw");
	circle.style.setProperty("--pos-y", Math.floor(Math.random() * 100) + "vh");
	circle.style.setProperty(
		"--end-x",
		Math.floor(Math.random() * 100) - 50 + "vw"
	);
	circle.style.setProperty(
		"--end-y",
		Math.floor(Math.random() * 100) - 50 + "vh"
	);

	setTimeout(() => {
		circle.remove();
	}, 8500);

	bg.appendChild(circle);
}
