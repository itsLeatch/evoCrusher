extends Node2D

@export var botCount: int = 10
@export var borderSize: float = 64

var BorderScene: PackedScene = load("res://border.tscn")
var BotScene: PackedScene = load("res://Bot.tscn")
var FoodScene: PackedScene = load("res://food.tscn")
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#create border
	#top
	var topBorder = BorderScene.instantiate()
	$Borders.add_child(topBorder)
	topBorder.global_position = Vector2(0,-Global.mapSize.y / 2 - borderSize / 2)
	topBorder.scale = Vector2(Global.mapSize.x,borderSize)
	#bottom
	var bottomBorder = BorderScene.instantiate()
	$Borders.add_child(bottomBorder)
	bottomBorder.global_position = Vector2(0,Global.mapSize.y / 2 + borderSize / 2)
	bottomBorder.scale = Vector2(Global.mapSize.x,borderSize)
	#left
	var leftBorder = BorderScene.instantiate()
	$Borders.add_child(leftBorder)
	leftBorder.global_position = Vector2(-Global.mapSize.x / 2 - borderSize / 2,0)
	leftBorder.scale = Vector2(borderSize,Global.mapSize.y)
	#right
	var rightBorder = BorderScene.instantiate()
	$Borders.add_child(rightBorder)
	rightBorder.global_position = Vector2(Global.mapSize.x / 2 + borderSize / 2,0)
	rightBorder.scale = Vector2(borderSize,Global.mapSize.y)
	#create bots
	for i in range(botCount):
		var bot = BotScene.instantiate()
		$Bots.add_child(bot)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	


func _on_timer_timeout() -> void:
	var food = FoodScene.instantiate()
	$Food.add_child(food)
	
