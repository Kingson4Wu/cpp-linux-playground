// @ts-check
import {themes as prismThemes} from 'prism-react-renderer';

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: 'Linux C++ Backend Development Playground | Master C++ Programming',
  tagline: 'Master Modern C++, System Programming, Network Programming, and Performance Optimization | Comprehensive C++ Learning Resource',
  favicon: 'img/logo.svg',

  // Set the production url of your site here
  url: 'https://Kingson4Wu.github.io',
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: '/cpp-linux-playground/',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'Kingson4Wu', // Usually your GitHub org/user name.
  projectName: 'cpp-linux-playground', // Usually your repo name.

  onBrokenLinks: 'warn',
  onBrokenMarkdownLinks: 'warn',

  // SEO and meta tags optimization
  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          sidebarPath: './sidebars.js',
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/Kingson4Wu/cpp-linux-playground/tree/main/documentation/website/',
          // Generate clean URLs with trailing slash
          routeBasePath: 'docs',
          // Include draft documents in search
          includeCurrentVersion: true,
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/Kingson4Wu/cpp-linux-playground/tree/main/documentation/website/',
          feedOptions: {
            type: ['rss', 'atom'],
          },
        },
        theme: {
          customCss: './src/css/custom.css',
        },
      }),
    ],
  ],

  scripts: [
    {
      src: 'https://static.cloudflareinsights.com/beacon.min.js',
      defer: true,
      'data-cf-beacon': '{"token": "40cb59600d874d4ebe8da6d9367b4394"}',
    },
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      // Replace with your project's social card
      image: 'img/logo.svg',
      metadata: [
        {name: 'keywords', content: 'C++, Linux, backend development, system programming, network programming, performance optimization, tutorial, guide, documentation'},
        {name: 'description', content: 'Master Modern C++, System Programming, Network Programming, and Performance Optimization through progressive hands-on projects. Comprehensive C++ learning resource for backend developers.'},
        {name: 'og:title', content: 'Linux C++ Backend Development Playground | Master C++ Programming'},
        {name: 'og:description', content: 'Comprehensive C++ learning resource covering system programming, network programming, and performance optimization for backend developers.'},
        {name: 'og:type', content: 'website'},
        {name: 'og:url', content: 'https://Kingson4Wu.github.io/cpp-linux-playground/'},
        {name: 'og:image', content: 'img/logo.svg'},
        {name: 'twitter:card', content: 'summary_large_image'},
        {name: 'twitter:site', content: '@github'},
        {name: 'twitter:title', content: 'Linux C++ Backend Development Playground'},
        {name: 'twitter:description', content: 'Master Modern C++, System Programming, Network Programming, and Performance Optimization'},
      ],
      navbar: {
        title: 'C++ Linux Playground',
        logo: {
          alt: 'C++ Logo',
          src: 'img/logo.svg',
        },
        items: [
          {
            to: '/',
            label: 'Home',
            position: 'left'
          },
          {
            type: 'docSidebar',
            sidebarId: 'tutorialSidebar',
            position: 'left',
            label: 'Documentation',
          },
          {to: '/blog', label: 'Blog', position: 'left'},
          {
            href: 'https://github.com/Kingson4Wu/cpp-linux-playground',
            label: 'GitHub',
            position: 'right',
          },
        ],
      },
      footer: {
        style: 'dark',
        links: [
          {
            title: 'Docs',
            items: [
              {
                label: 'Tutorial',
                to: '/docs/intro',
              },
            ],
          },
          {
            title: 'Community',
            items: [
              {
                label: 'Stack Overflow',
                href: 'https://stackoverflow.com/questions/tagged/docusaurus',
              },
              {
                label: 'Discord',
                href: 'https://discordapp.com/invite/docusaurus',
              },
              {
                label: 'Twitter',
                href: 'https://twitter.com/docusaurus',
              },
            ],
          },
          {
            title: 'More',
            items: [
              {
                label: 'Blog',
                to: '/blog',
              },
              {
                label: 'GitHub',
                href: 'https://github.com/Kingson4Wu/cpp-linux-playground',
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} C++ Linux Playground Project. Built with Docusaurus.`,
      },
      prism: {
        theme: prismThemes.github,
        darkTheme: prismThemes.dracula,
        additionalLanguages: ['cpp', 'cmake', 'bash'],
      },
      // Enhanced search for better SEO
      algolia: {
        // The application ID provided by Algolia
        appId: 'YOUR_ALGOLIA_APP_ID',
        // Public API key: it is safe to commit it
        apiKey: 'YOUR_ALGOLIA_API_KEY',
        indexName: 'cpp-linux-playground',
        // Optional: see doc section below
        contextualSearch: true,
        // Optional: Specify domains where the navigation should occur through window.location instead on history.push. Useful when our Algolia config crawls multiple documentation sites and we want to navigate with window.location.href to them.
        externalUrlRegex: 'external\\.com|domain\\.com',
        // Optional: Algolia search parameters
        searchParameters: {},
        // Optional: path for search page that enabled by default (`false` to disable it)
        searchPagePath: 'search',
      },
    }),
};

export default config;