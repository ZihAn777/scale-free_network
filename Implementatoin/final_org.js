// set some element
var svg2 = d3.select("#org_graph");
var width2 = svg2.attr("width");
var height2 = svg2.attr("height");

var url_or = "https://raw.githubusercontent.com/ZihAn777/scale-free_network/master/edges.json";


var g_links2 = JSON.parse(Get(url_or));
var g_nodes2 = JSON.parse(Get(url_node));

// set the simulation
var simulation2 = d3
	.forceSimulation(g_nodes2)
	.force(
		"link", 
		d3.forceLink(g_links2)
	)
	.force("charge", d3.forceManyBody().strength(-0.5))
	.force("center", d3.forceCenter(width2/2, height2/2))
	.on("tick", ticked);

// the links
var link2 = svg2
	.append("g")
	.selectAll("line")
	.data(g_links2)
	.enter()
	.append("line")
	.attr("stroke-width", function(d) {
		return 0.3;
	})
	.style("stroke", "pink");
// the nodes
var node2 = svg2
	.append("g")
	.selectAll("circle")
	.data(g_nodes2)
	.enter()
	.append("circle")
	.attr("r", 0.8)
	.attr("fill", function(d) {
		return "orange";
	})
	.attr("stroke", "yellow");

// tick
function ticked() {
	link2
		.attr("x1", function(d) {
			return d.source.x;
		})
		.attr("y1", function(d) {
			return d.source.y;
		})
		.attr("x2", function(d) {
			return d.target.x;
		})
		.attr("y2", function(d) {
			return d.target.y;
		});

	node2
		.attr("cx", function(d) {
			return d.x;
		})
		.attr("cy", function(d) {
			return d.y;
		});
}