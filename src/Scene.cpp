#include "Scene.h"
#include "Class.h"

/*
* descriptor
{
"Classes":{
"human":{ ->QJsonObject
	"Data":[["+","age"],
	["+","name"],
	["+","heght"]],
	"Methods":[["+","GetAge():number"]],
	"Alias": "H"
},
"human2":{ ->QJsonObject
	"Data":[["+","age"],
	["+","name"],
	["+","heght"]],
	"Methods":[["+","GetAge():number"]],
	"Alias": "H2"
},
"Connections":[[1,2,con_t]]
}
}
*/

Scene::Scene(QJsonObject doc)
	:view(&scene)
{
	if (doc.contains("Classes"))
	{
		auto&& o = doc["Classes"].toObject();
		auto ks = o.keys();

		for (auto&& i : ks)
		{
			scene.addItem(new Class(std::move(i), o[i].toObject()));
		}
	}
}