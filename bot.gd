extends CharacterBody2D

class_name Bot

#input for each sensor:
	#each sensor contains following data
	# distance
	# isFood
	# isOtherBot
	# isBoarder
	
const sensorInputCount:int = 4
#outputData
# velocityX
# velocityY


const sensorOutputCount: int = 1

@export var speed :float = 300
@export var sensorLength : float = 100
@export var sensorCount : int = 3
@export var sensorArea : float = PI / 2

var weights :Float32Matrix = Float32Matrix.new(Vector2i(sensorInputCount * sensorCount,sensorOutputCount))
var inputData :Float32Matrix =  Float32Matrix.new(Vector2i(1,sensorInputCount * sensorCount))
var energy: float = 0
var sensors: Array = []

# Called when the node enters the scene tree for the first time.
var rng = RandomNumberGenerator.new()
var hitResult;

func _ready() -> void:
	var randomWeightData = PackedFloat32Array()
	print(weights.rect())
	for i in range(weights.x * weights.y):
		randomWeightData.append(rng.randf_range(-1,1))
	weights.data = randomWeightData
	#print(weights.data)
	global_position = Vector2(rng.randf_range(-Global.mapSize.x / 2,Global.mapSize.x / 2), rng.randf_range(-Global.mapSize.y / 2,Global.mapSize.y / 2))
	for i in range(sensorCount):
		sensors.append(Line2D.new())
		sensors[sensors.size() - 1].width = 5
		$Sensors.add_child(sensors[sensors.size() - 1])
 

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	var sensorData = []
	#ray cast
	var space_state = get_world_2d().direct_space_state
	for i in range(sensors.size()):
		var angle = 0
		if sensors.size() > 1:
			angle = -sensorArea / 2+ sensorArea / (sensorCount - 1) * i
		
		var endPoint = global_position + velocity.rotated(angle) / velocity.length() * sensorLength
		if is_nan(endPoint.x) || is_nan(endPoint.y):
			endPoint = Vector2(0,0)
		var query = PhysicsRayQueryParameters2D.create(global_position, endPoint, collision_mask)
		query.collide_with_areas = true
		hitResult = space_state.intersect_ray(query)
		#if hitResult:
		var hitVertices: PackedVector2Array;
		hitVertices.append(Vector2(0,0)) 
		if hitResult:
			hitVertices.append(hitResult.position - global_position)
			sensors[i].default_color = Color.RED
			
			sensorData.append((hitResult.position - global_position).distance_to(Vector2(0,0)))
			sensorData.append(hitResult.collider is Food)
			sensorData.append(hitResult.collider is Bot)
			sensorData.append(hitResult.collider is Border)
			
			
		else:
			hitVertices.append(endPoint - global_position)
			sensors[i].default_color = Color.GREEN
			var pos : Vector2 = endPoint - global_position
			sensorData.append(pos.distance_to(Vector2(0,0)))
			sensorData.append(0)
			sensorData.append(0)
			sensorData.append(0)
			
		sensors[i].points = hitVertices
	#get sensor data
	inputData.data = sensorData
	
	#foreward pass
	var result = weights.mul(inputData)
	var resVelocity = Vector2(result.get_value(Vector2(0,0)), result.get_value((Vector2(0,1))))
	resVelocity /= resVelocity.length()
	
	#move
	velocity = resVelocity * speed
	move_and_slide()
	$Label.text = str(energy)

func incrementFood() ->void:
	energy+=1


func _on_time_alive_timeout() -> void:
	energy-=1
	if(energy < 0):
		queue_free()
