// set some element
var svg1 = d3.select("#sim_graph");
var width1 = svg1.attr("width");
var height1 = svg1.attr("height");

var url_link = "https://raw.githubusercontent.com/zih-an/scale-free_network/master/fb_newLink.json";
var url_node = "https://raw.githubusercontent.com/zih-an/scale-free_network/master/nodes.json";

function Get(yourUrl){
    var Httpreq = new XMLHttpRequest(); // a new request
    Httpreq.open("GET",yourUrl,false);
    Httpreq.send(null);
    return Httpreq.responseText;          
}

var g_links1 = JSON.parse(Get(url_link));
var g_nodes1 = JSON.parse(Get(url_node));


// set the simulation
var simulation1 = d3
	.forceSimulation(g_nodes1)
	.force(
		"link", 
		d3.forceLink(g_links1)
	)
	.force("charge", d3.forceManyBody().strength(-0.5))
	.force("center", d3.forceCenter(width1/2, height1/2))
	.on("tick", ticked);

// the links
var link1 = svg1
	.append("g")
	.selectAll("line")
	.data(g_links1)
	.enter()
	.append("line")
	.attr("stroke-width", function(d) {
		return 0.8;
	})
	.style("stroke", "pink");
// the nodes
var node1 = svg1
	.append("g")
	.selectAll("circle")
	.data(g_nodes1)
	.enter()
	.append("circle")
	.attr("r", 0.5)
	.attr("fill", function(d) {
		return "orange";
	})
	.attr("stroke", "yellow");


// tick
function ticked() {
	link1
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

	node1
		.attr("cx", function(d) {
			return d.x;
		})
		.attr("cy", function(d) {
			return d.y;
		});
}
