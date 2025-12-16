/** @type {import('@docusaurus/plugin-content-docs').SidebarsConfig} */
const sidebars = {
  tutorialSidebar: [
    'intro',
    {
      type: 'category',
      label: 'Getting Started',
      items: [
        'getting-started/setup',
        'getting-started/project-overview',
        'getting-started/docker-setup',
      ],
    },
    {
      type: 'category',
      label: 'Phase 1: Syntax and Tool Familiarization',
      items: [
        'phase1/intro',
        'phase1/cli-tools',
        'phase1/json-parser',
        'phase1/logger',
      ],
    },
    {
      type: 'category',
      label: 'Phase 2: System Programming',
      items: [
        'phase2/intro',
        'phase2/memory-pool',
        'phase2/process-manager',
        'phase2/threaded-downloader',
      ],
    },
    {
      type: 'category',
      label: 'Phase 3: Network Programming',
      items: [
        'phase3/intro',
        'phase3/tcp-chat-room',
        'phase3/http-server',
        'phase3/tcp-file-transfer',
      ],
    },
    {
      type: 'category',
      label: 'Phase 4: Comprehensive Practice',
      items: [
        'phase4/intro',
        'phase4/mini-redis',
        'phase4/mini-search',
        'phase4/crawler',
      ],
    },
    {
      type: 'category',
      label: 'Development Standards',
      items: [
        'standards/code-style',
        'standards/testing',
        'standards/git-commit',
      ],
    },
  ],
};

export default sidebars;