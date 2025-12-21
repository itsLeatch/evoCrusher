extends CharacterBody2D

class_name Bot
#input for each sensor:
	#each sensor contains following data
	# distance
	# isFood
	# isOtherBot
	# isBoarder
#general input
	#bias

const sensorInputCount:int = 4
const generalSensorInputCount:int = 1
	
#outputData
# rotation


const outputCount: int = 1

@export var speed :float = 50
@export var maximalSensorLength : float = 100
@export var sensorCount : int = 2
@export var sensorArea : float = PI / 2
@export var maximumEnergy: int = 10
@export var energyForReproduction :int = 1
@export var minEnergyToStartReproduction = 4

var neuronsPerLayer = [sensorInputCount * sensorCount + generalSensorInputCount, 6, outputCount]
var neuronWeights :Array = []

var inputData :Float32Matrix =  Float32Matrix.new(Vector2i(1,neuronsPerLayer[0]))

var energy: float = 0
var sensors: Array = []

# Called when the node enters the scene tree for the first time.
var rng = RandomNumberGenerator.new()
var hitResult;

var internalRotation:float = 0

func _ready() -> void:
	#Engine.physics_ticks_per_second = 4
	for i in range(neuronsPerLayer.size() - 1):
		var randomWeightData = PackedFloat32Array()
		var neuronLayer: Float32Matrix = Float32Matrix.new(Vector2(neuronsPerLayer[i], neuronsPerLayer[i+1]))
		for j in range(neuronLayer.x * neuronLayer.y):
			randomWeightData.append(rng.randf_range(-1,1))
		neuronLayer.data = randomWeightData
		neuronWeights.append(neuronLayer)
	#print(weights.data)
	global_position = Vector2(rng.randf_range(-Global.currentLevel.mapSize.x / 2,Global.currentLevel.mapSize.x / 2), rng.randf_range(-Global.currentLevel.mapSize.y / 2,Global.currentLevel.mapSize.y / 2))
	for i in range(sensorCount):
		sensors.append(Line2D.new())
		sensors[sensors.size() - 1].width = 5
		$Sensors.add_child(sensors[sensors.size() - 1])
	internalRotation = rng.randf_range(-PI, PI)
 

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var sensorData = []
	#ray cast
	var space_state = get_world_2d().direct_space_state
	for i in range(sensors.size()):
		var angle = 0
		if sensors.size() > 1:
			angle = -sensorArea / 2+ sensorArea / (sensorCount - 1) * i
		
		var endPoint = global_position + velocity.rotated(angle) / velocity.length() * maximalSensorLength
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
			var currentSensorLength = (hitResult.position - global_position).distance_to(Vector2(0,0))
			sensorData.append(currentSensorLength / maximalSensorLength)
			if hitResult.collider is Food:				
				sensorData.append(1)
			else:
				sensorData.append(0)
			if hitResult.collider is Bot:				
				sensorData.append(1)
			else:
				sensorData.append(0)
			if hitResult.collider is Border:				
				sensorData.append(1)
			else:
				sensorData.append(0)
							
			
		else:
			hitVertices.append((endPoint - global_position))
			sensors[i].default_color = Color.GREEN
			var pos : Vector2 = endPoint - global_position
			sensorData.append(pos.distance_to(Vector2(0,0)) / maximalSensorLength)
			sensorData.append(0)
			sensorData.append(0)
			sensorData.append(0)
			
		sensors[i].points = hitVertices
	#get sensor data
	inputData.data = sensorData
	#set the bias
	inputData.data.append(1)
	
	#foreward pass
	var resultOfPreviousCalculation :Float32Matrix = neuronWeights[0].mul(inputData) 
	for i in range(neuronWeights.size() - 1):
		resultOfPreviousCalculation = neuronWeights[i + 1].mul(resultOfPreviousCalculation)
	internalRotation += resultOfPreviousCalculation.get_value(Vector2(0,0)) * delta
	$BotImage.rotation = internalRotation
	
	#move
	velocity = Vector2(0,-1).rotated(internalRotation) * speed
	var colisionInfo = move_and_collide(velocity * delta)
	
	
	
	#check for collision with other player for reproduction
	if colisionInfo && colisionInfo.get_collider() is Bot:
		if(energy >= minEnergyToStartReproduction && colisionInfo.get_collider().getEnergy() >= minEnergyToStartReproduction):
			var newNeurons :Array = []
			for i in range(neuronWeights.size()):
				var newWeights: PackedFloat32Array
				for j in range(neuronWeights[i].data.size()):
					var ownInfluence = energy / (energy + colisionInfo.get_collider().getEnergy()) # how much the weight is effected by the current bot: 0 - all from other bot, 0.5 half, half, 1 - all from current
					var mutation = 0 #rng.randf_range(-0.025,0.025) # mutation rate
					var mixedWeight = (neuronWeights[i].data[j] * ownInfluence + colisionInfo.get_collider().getWeights()[i].data[j] * (1 - ownInfluence)) / 2
					var weightWithMutation = mixedWeight + mutation
					if weightWithMutation > 1:
						weightWithMutation = 1
					elif weightWithMutation < -1:
						weightWithMutation = -1 
					newWeights.append(weightWithMutation)
				
				var newNeuronLayer : Float32Matrix = Float32Matrix.new(Vector2(neuronWeights[i].x,neuronWeights[i].y))
				newNeuronLayer.data = newWeights
				newNeurons.append(newNeuronLayer)
				
			Global.currentLevel.spawnNewPlayer(newNeurons)
			energy -= energyForReproduction
			colisionInfo.get_collider().decreaseEnergy(energyForReproduction)
	#print current energy 
	$Label.text = str(energy)
	
func incrementFood() ->void:
	energy+=1
	if energy > maximumEnergy:
		energy = maximumEnergy

func decreaseEnergy(value: float) -> void:
	energy -= value
	
func getEnergy() -> float:
	return energy

func getWeights() -> Array:
	return neuronWeights

func setWeights(newWeights: Array) -> void:
	neuronWeights = newWeights


func _on_time_alive_timeout() -> void:
	energy-=1
	if(energy < 0):
		queue_free()
