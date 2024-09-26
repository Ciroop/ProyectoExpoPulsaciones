import React, { useRef, useState } from 'react';
import Canvas from './components/Canvas';
import Toolbar from './components/Toolbar';
import './index.css'; // Asegúrate de que este archivo se importe

const App = () => {
  const canvasRef = useRef(null);
  const [color, setColor] = useState('#000000');
  const [lineWidth, setLineWidth] = useState(10); // Grosor inicial
  const [traces, setTraces] = useState([]); // Almacena los trazos dibujados
  const [isDrawing, setIsDrawing] = useState(false); // Estado para verificar si se está dibujando
  const [currentTrace, setCurrentTrace] = useState({ points: [], sprays: [], color, lineWidth }); // Almacena el trazo actual

  const clearCanvas = () => {
    const canvas = canvasRef.current; //sdsd
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
    loadBackground(); // Cargar la imagen de fondo
    setTraces([]); // Limpiar el historial de trazos
  };

  const loadBackground = () => {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    const img = new Image();
    img.src = '/path/to/background/image.jpg'; // Ruta de la imagen de fondo
    img.onload = () => {
      context.drawImage(img, 0, 0, canvas.width, canvas.height);
    };
  };

  const startDrawing = (event) => {
    setIsDrawing(true);
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');

    const rect = canvas.getBoundingClientRect(); // Obtiene el rectángulo del lienzo
    const startX = event.clientX - rect.left; // Calcula la posición X relativa al lienzo
    const startY = event.clientY - rect.top; // Calcula la posición Y relativa al lienzo

    context.lineWidth = lineWidth;
    context.strokeStyle = color;
    context.lineCap = 'round';
    context.beginPath();
    context.moveTo(startX, startY); // Usa las coordenadas ajustadas

    // Inicializa el trazo actual con su color y grosor
    setCurrentTrace({ points: [{ x: startX, y: startY }], sprays: [], color, lineWidth });
  };

  const draw = (event) => {
    if (!isDrawing) return;
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');

    const rect = canvas.getBoundingClientRect(); // Obtiene el rectángulo del lienzo
    const x = event.clientX - rect.left; // Calcula la posición X relativa al lienzo
    const y = event.clientY - rect.top; // Calcula la posición Y relativa al lienzo

    // Trazo continuo
    context.lineTo(x, y);
    context.stroke();

    // Generar spray
    const sprays = generateSpray(x, y, lineWidth); // Genera las gotas de spray

    // Dibuja el spray en la posición actual
    sprays.forEach(({ offsetX, offsetY, radius }) => {
      context.beginPath();
      context.arc(x + offsetX, y + offsetY, radius, 0, Math.PI * 2);
      context.fillStyle = color;
      context.fill();
    });

    // Agrega el punto actual al trazo y las gotas de spray
    setCurrentTrace((prev) => ({
      ...prev,
      points: [...prev.points, { x, y }],
      sprays: [...prev.sprays, sprays], // Almacena las gotas de spray
    }));
  };

  const generateSpray = (x, y, lineWidth) => {
    const numSprayDots = 8; // Reducir el número de puntos para una cobertura más sutil
    const sprays = [];

    for (let i = 0; i < numSprayDots; i++) {
      const offsetX = (Math.random() - 0.5) * (lineWidth / 2); // Desplazamiento menor en X
      const offsetY = (Math.random() - 0.5) * (lineWidth / 2); // Desplazamiento menor en Y
      const radius = (lineWidth / 6); // Radio más pequeño para un efecto más sutil

      sprays.push({ offsetX, offsetY, radius });
    }

    return sprays;
  };

  const stopDrawing = () => {
    if (isDrawing) {
      const newTraces = [...traces, currentTrace]; // Agrega el trazo actual al historial
      setTraces(newTraces); // Actualiza el historial
      setCurrentTrace({ points: [], sprays: [], color, lineWidth }); // Resetea el trazo actual
    }
    setIsDrawing(false);
  };

  const undo = () => {
    if (traces.length > 0) {
      const newTraces = traces.slice(0, -1); // Elimina el último trazo de la pila
      setTraces(newTraces); // Actualiza el historial
      redrawCanvas(newTraces); // Redibuja el lienzo
    }
  };

  const redrawCanvas = (tracesToRedraw) => {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');

    context.clearRect(0, 0, canvas.width, canvas.height); // Limpia el lienzo
    loadBackground(); // Vuelve a cargar la imagen de fondo

    tracesToRedraw.forEach(({ points, sprays, color, lineWidth }) => {
      context.strokeStyle = color; // Establece el color del trazo
      context.lineWidth = lineWidth; // Establece el grosor del trazo
      context.beginPath(); // Comienza un nuevo trazo
      context.moveTo(points[0].x, points[0].y); // Comienza desde el primer punto

      // Redibuja el trazo
      points.forEach((point, index) => {
        context.lineTo(point.x, point.y);
        context.stroke(); // Dibuja el trazo

        // Dibuja las gotas de spray previamente almacenadas
        if (sprays[index]) { // Verifica que sprays[index] existe
          sprays[index].forEach(({ offsetX, offsetY, radius }) => {
            context.beginPath();
            context.arc(point.x + offsetX, point.y + offsetY, radius, 0, Math.PI * 2);
            context.fillStyle = color;
            context.fill();
          });
        }
      });

      context.stroke(); // Finaliza el trazo
    });
  };

  return (
    <div style={{ display: 'flex', height: '100vh' }}>
      <Toolbar setColor={setColor} setLineWidth={setLineWidth} clearCanvas={clearCanvas} undo={undo} />
      <Canvas
        ref={canvasRef}
        color={color}
        lineWidth={lineWidth}
        startDrawing={startDrawing}
        draw={draw}
        stopDrawing={stopDrawing}
      />
    </div>
  );
};

export default App;
