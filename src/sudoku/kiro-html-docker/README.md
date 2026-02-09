# Sudoku Web Application

A modern, single-file Sudoku game with an enhanced UI that runs in any web browser.

## Features

### Core Gameplay
- 9x9 Sudoku grid with classic layout
- Four built-in puzzles with random selection
- Real-time input validation (accepts only numbers 1-9)
- Solution checker with error highlighting
- Fixed cells are pre-filled and locked
- Auto-save game state (resume anytime)

### UI/UX Enhancements
- Modern, eye-friendly design with purple gradient theme
- Fully responsive layout (works on desktop and mobile)
- Visual feedback for selected cells, valid/invalid entries
- Smooth animations for cell selection and number placement
- Game info panel displaying:
  - Difficulty level
  - Live timer
  - Mistake counter
  - Completion percentage with progress bar

### Controls
- Mouse: Click cells to select and type numbers
- Keyboard: Arrow keys to navigate, numbers 1-9 to fill cells

## How to Play

1. Open `sudoku.html` in any web browser
2. Fill empty cells with numbers 1-9
3. Each row, column, and 3x3 box must contain digits 1-9 without repetition
4. Click "Check" to validate your solution
5. Click "New Game" to start a fresh puzzle

## Technical Details

- Pure HTML/CSS/JavaScript - no dependencies
- Single file application
- Works offline
- Responsive 50x50px cells with visual 3x3 box separation

## Project Structure

```
.
├── sudoku.html          # Main application file
├── Dockerfile           # Docker image configuration
├── docker-compose.yml   # Docker Compose setup
├── .gitignore          # Git ignore rules
└── README.md           # This file
```

## Usage

### Docker (Recommended)
```bash
# Using docker-compose
docker-compose up -d

# Or using Docker directly
docker build -t sudoku .
docker run -d -p 8080:80 sudoku
```
Access at http://localhost:8080

### Local
```bash
# Open directly in browser
open sudoku.html

# Or serve with any HTTP server
python -m http.server 8000
```
