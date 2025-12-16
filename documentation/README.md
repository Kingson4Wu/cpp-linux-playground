# C++ Linux Playground Documentation

This directory contains the documentation website for the Linux C++ Backend Development Playground project, built with Docusaurus.

## Structure

- `docs/` - All documentation content organized by learning phases
- `src/` - Custom React components and pages (including the homepage)
- `static/` - Static assets like images and icons
- `blog/` - Blog posts and updates
- `docusaurus.config.js` - Main configuration for the site
- `sidebars.js` - Navigation sidebar configuration

## Local Development

To run the documentation site locally:

```bash
cd documentation/website
npm start
```

This will start a development server accessible at http://localhost:3000/cpp-linux-playground/

## Building

To create a production build:

```bash
cd documentation/website
npm run build
```

The static site will be generated in the `build/` directory.

## Deployment

The site is automatically deployed to GitHub Pages via the workflow in `.github/workflows/documentation.yml` when changes are pushed to the main branch.