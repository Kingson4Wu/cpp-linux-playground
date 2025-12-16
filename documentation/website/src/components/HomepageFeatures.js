import React from 'react';
import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './HomepageFeatures.module.css';

const FeatureList = [
  {
    title: 'Progressive Learning',
    Svg: require('@site/static/img/logo.svg').default,
    description: (
      <>
        Master C++ through four progressive phases: from syntax and tools to 
        comprehensive real-world applications.
      </>
    ),
  },
  {
    title: 'Hands-on Practice',
    Svg: require('@site/static/img/logo.svg').default,  // We'll replace this with proper SVG later
    description: (
      <>
        Build practical projects like TCP chat rooms, HTTP servers, 
        memory pools, and search engines.
      </>
    ),
  },
  {
    title: 'Industry Standards',
    Svg: require('@site/static/img/logo.svg').default,
    description: (
      <>
        Learn modern C++20, system programming, network programming, 
        and best practices used in production environments.
      </>
    ),
  },
];

function Feature({Svg, title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}