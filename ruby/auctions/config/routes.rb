RorAuctions::Application.routes.draw do
  resources :users
  resources :auctions
  resources :sessions, only: [:new, :create, :destroy]
  resources :bids, only: [:create]
  
  match '/auctions/:id/finish', to: 'Auctions#finish', as: :finish_auction
  match '/signup', to: 'Users#new'
  match '/signin', to: 'Sessions#new'
  match '/signout', to: 'Sessions#destroy', via: :delete

  match '/tags/:tag', to: 'Auctions#index', as: :tag

  root to: 'StaticPages#home'
end
