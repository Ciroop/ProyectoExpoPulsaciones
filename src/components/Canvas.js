import React, { useEffect, forwardRef } from 'react';

const Canvas = forwardRef(({ color, lineWidth, startDrawing, draw, stopDrawing }, ref) => {
  useEffect(() => {
    const canvas = ref.current;
    const context = canvas.getContext('2d');

    // Cargar la imagen de fondo
    loadBackground();

    const drawOnCanvas = (event) => {
      requestAnimationFrame(() => draw(event)); // Llama a draw usando requestAnimationFrame
    };

    canvas.addEventListener('mousedown', startDrawing);
    canvas.addEventListener('mousemove', drawOnCanvas);
    canvas.addEventListener('mouseup', stopDrawing);
    canvas.addEventListener('mouseleave', stopDrawing);

    return () => {
      canvas.removeEventListener('mousedown', startDrawing);
      canvas.removeEventListener('mousemove', drawOnCanvas);
      canvas.removeEventListener('mouseup', stopDrawing);
      canvas.removeEventListener('mouseleave', stopDrawing);
    };
  }, [color, lineWidth, ref, startDrawing, draw, stopDrawing]);

  const loadBackground = () => {
    const canvas = ref.current;
    const context = canvas.getContext('2d');
    const img = new Image();
    img.src = '/path/to/background/image.jpg'; // Reemplaza con la ruta de tu imagen
    img.onload = () => {
      context.drawImage(img, 0, 0, canvas.width, canvas.height);
    };
  };

  return (
    <canvas 
      ref={ref} 
      width={window.innerWidth} 
      height={window.innerHeight} 
      style={{ 
        border: '1px solid black', 
        display: 'block', 
        position: 'relative', 
        zIndex: 0 // Asegúrate de que el lienzo esté detrás de la barra
      }} 
    />
  );
});

export default Canvas;
