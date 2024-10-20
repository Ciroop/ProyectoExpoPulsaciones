import React, { useState } from 'react';
import 'bootstrap-icons/font/bootstrap-icons.css';

const Toolbar = ({ setColor, setLineWidth, clearCanvas, undo, saveDrawing,  }) => {
  const [isVisible, setIsVisible] = useState(false);

  const handleMouseEnter = () => {
    setIsVisible(true);
  };

  const handleMouseLeave = () => {
    setIsVisible(false);
  };

  return (
    <div 
      style={{ 
        display: 'flex',
        flexDirection: 'column',
        justifyContent: 'space-between',
        height: '100vh',
        backgroundColor: '#ffffff', 
        borderRight: '1px solid #ccc',
        position: 'relative',
        zIndex: 1,
        boxShadow: '2px 0 5px rgba(0, 0, 0, 0.1)',
      }}
    >
      <div 
        style={{
          position: 'absolute',
          left: '0',
          top: '0',
          width: '30px',
          height: '100%',
          zIndex: 0,
        }}
        onMouseEnter={handleMouseEnter}
        onMouseLeave={handleMouseLeave}
      />
      
      <div 
        style={{ 
          width: isVisible ? '60px' : '0',
          overflow: 'hidden', 
          padding: isVisible ? '10px' : '0', 
          transition: 'width 0.3s, padding 0.3s',
        }}
        onMouseEnter={handleMouseEnter}
        onMouseLeave={handleMouseLeave}
      >
        {/* Color Picker con Icono */}
        <div style={{ marginBottom: '20px', textAlign: 'center' }}>
          <input 
            type="color" 
            onChange={(e) => setColor(e.target.value)} 
            style={{ border: 'none', width: '30px', height: '30px' }} 
            title="Color"
          />
          <span className="bi bi-paint-bucket" title="Balde de pintura" />
        </div>

        {/* Grosor con Icono */}
        <div style={{ marginBottom: '20px', textAlign: 'center' }}>
          <input 
            type="range" 
            min="1" 
            max="20" 
            onChange={(e) => setLineWidth(e.target.value)} 
            style={{ width: '100%' }} 
            title="Grosor"
          />
          <span className="bi bi-pencil" title="Lápiz" />
        </div>

        {/* Botón de Limpiar con Icono */}
        <div style={{ marginBottom: '20px', textAlign: 'center' }}>
          <button onClick={clearCanvas} style={buttonStyle} title="Limpiar">
            <i className="bi bi-eraser" />
          </button>
        </div>

        {/* Botón de Deshacer con Icono */}
        <div style={{ textAlign: 'center', marginTop: '20px' }}>
          <button onClick={undo} style={buttonStyle} title="Deshacer">
            <i className="bi bi-arrow-counterclockwise" />
          </button>
        </div>

        <div style={{ textAlign: 'center', marginTop: '20px' }}>
          <button onClick={saveDrawing} style={buttonStyle} title="Guardar">
            <i className="bi bi-save" />
          </button>
        </div>

      </div>
    </div>
  );
};

// Estilo para los botones
const buttonStyle = {
    background: 'none',
    border: 'none',
    cursor: 'pointer',
    fontSize: '20px', 
    color: '#333', 
    transition: 'color 0.3s',
    margin: '10px 0', // Añadir margen para espaciar los botones
  };
  
export default Toolbar;
